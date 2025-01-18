// QYF Legal Description


#include "Actor\AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// #include "AbilitySystemComponent.h"
// #include "AbilitySystemInterface.h"
// #include "AbilitySystem/AuraAttributeSet.h"
// #include "Components/SphereComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	
	// StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	// SetRootComponent(StaticMeshComp);
	//
	// Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	// Sphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	// Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	// Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnEndOverlap);
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;
	
	check(GameplayEffectClass);
	FGameplayEffectContextHandle GameplayEffectContextHandle = TargetASC->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpec = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, GameplayEffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
	const bool bIsInfinite = EffectSpec.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		// TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		// for (const TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		// {
		// 	if (HandlePair.Value == TargetASC)
		// 	{
		// 		TargetASC->RemoveActiveGameplayEffect(HandlePair.Key);
		// 		HandlesToRemove.Add(HandlePair.Key);
		// 	}
		// }
		// for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		// {
		// 	ActiveEffectHandles.FindAndRemoveChecked(Handle);
		// }
		for (TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*>::TIterator iter(ActiveEffectHandles); iter; ++iter)
		{
			if (iter->Value == TargetASC)
			{
				// StacksToRemove=1表示只移除一层效果，默认-1表示全部移除
				TargetASC->RemoveActiveGameplayEffect(iter->Key, 1);
				iter.RemoveCurrent();
			}
		}
	}
}

// void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
// 	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	// TODO Change this to apply a Gameplay Effect. For now, Using const_cast as a Hook!
// 	if (const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
// 	{
// 		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
// 		UAuraAttributeSet* TempAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
// 		TempAuraAttributeSet->SetHealth(TempAuraAttributeSet->GetHealth() + 10.0f);
// 		TempAuraAttributeSet->SetMana(TempAuraAttributeSet->GetMana() - 10.0f);
// 		Destroy();
// 	}
// }
//
// void AAuraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// }


