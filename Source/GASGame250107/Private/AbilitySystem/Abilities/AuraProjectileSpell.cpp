// QYF Legal Description


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// UKismetSystemLibrary::PrintString(this, FString("ActivateAbility (C++)"), true, true, FLinearColor::Yellow, 3);

	// const bool bIsServer = HasAuthority(&ActivationInfo);

}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (bIsServer)
	{
		if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
		{
			const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
			FRotator Rotation = FRotationMatrix::MakeFromX(ProjectileTargetLocation - SocketLocation).Rotator();
			//Rotation.Pitch = 0.f; 不知道是什么原因，Dedicated Server模式下，Client端发射的火球高度比较高，去掉这段代码就正常了
			if (bOverridePitch) {
				Rotation.Pitch = PitchOverride;
			}
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rotation.Quaternion());

			// 为什么不用SpawnActor而使用SpawnActorDeferred呢？因为还要给他设置一个用来造成伤害的Gameplay Effect Spec
			AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
				SpawnTransform,
				GetOwningActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			// Give the Projectile a Gameplay Effect Spec for causing Damage
			const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
			FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
			EffectContextHandle.SetAbility(this);
			EffectContextHandle.AddSourceObject(Projectile);
			TArray<TWeakObjectPtr<AActor>> Actors; // 由于EffectContextHandle是局部变量，所以他其实是随着主体消亡而消亡的，因此他里面的成员变量不应该影响到GC（类似于一个观测者的身份），因此变量类型是TWeakObjectPtr
			Actors.Add(Projectile);
			EffectContextHandle.AddActors(Actors);
			FHitResult HitResult;
			HitResult.Location = ProjectileTargetLocation;
			EffectContextHandle.AddHitResult(HitResult);
			
			
			const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
			const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
			// 法一：const float ScaledDamage = Damage.AsInteger(GetAbilityLevel());
			// 法二：const float ScaledDamage = Damage.EvaluateCurveAtLevel();
			// 法三：const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
			for (auto& Pair : DamageTypes) {
				const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage); // SetByCaller本质上是一个键值对，key为Pair.Key，value为ScaledDamage
			}

			Projectile->DamageEffectSpecHandle = SpecHandle;
			Projectile->FinishSpawning(SpawnTransform);
		}
		
	}
}
