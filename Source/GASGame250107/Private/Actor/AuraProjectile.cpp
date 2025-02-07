// QYF Legal Description


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GASGame250107/GASGame250107.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework\Character.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	FCollisionResponseContainer CollisionResponseContainer;
	CollisionResponseContainer.SetResponse(ECC_WorldDynamic, ECR_Overlap);
	CollisionResponseContainer.SetResponse(ECC_WorldStatic, ECR_Overlap);
	CollisionResponseContainer.SetResponse(ECC_Pawn, ECR_Overlap);
	Sphere->SetCollisionResponseToChannels(CollisionResponseContainer);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	ProjectileMovementComp->InitialSpeed = 550.f;
	ProjectileMovementComp->MaxSpeed = 550.f;
	ProjectileMovementComp->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority()) // OnOverlap已经加了声音和特效生成，为什么在这里还要再加一遍？因为Client的OnOverlap和Server的Destroyed()次序不确定！有可能Server的Destroyed()已经同步过来了而Client的OnOverlap还未执行，那么此时就需要手动触发原先本该在OnOverlap中执行的逻辑了，不然Client端有可能还未触发该段逻辑就直接被Destroy了
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	}
	Super::Destroyed();
}

void AAuraProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DamageEffectSpecHandle.IsValid() && DamageEffectSpecHandle.Data.IsValid() && // 服务器才执行UAuraProjectileSpell::SpawnProjectile方法，所以Client端需要DamageEffectSpecHandle.IsValid()以及DamageEffectSpecHandle.Data.IsValid()（TODO: 教程中只写了DamageEffectSpecHandle.Data.IsValid()）
		DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor) {
		return;
	}

	// Client端不检测Simulated角色
	if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy) {
		return;
	}
	// Client端检测OtherActor是否是自己，如果是自己则return
	if (!DamageEffectSpecHandle.IsValid() && !DamageEffectSpecHandle.Data.IsValid()&&
		Cast<AActor>(UGameplayStatics::GetPlayerCharacter(this, 0)) == OtherActor) {
		return;
	}

	if (!bHit) { // Server端执行之后会复制给Client端执行，而有可能此时Client端自己已经执行了，所以需要避免这种情况
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	}

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	} else
	{
		bHit = true;
	}
}


