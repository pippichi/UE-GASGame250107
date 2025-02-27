// QYF Legal Description


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "AuraGameplayTags.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "AbilitySystem\AuraAbilitySystemLibrary.h"
#include "GASGame250107/GASGame250107.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AI\AuraAIController.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "BehaviorTree\BehaviorTree.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	AbilitySystemComp = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}

void AAuraEnemy::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController) AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::Die(DeathImpulse);
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority()) {
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComp, CharacterClass);
	}

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComp->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &ThisClass::HitReactTagChange);

		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AAuraEnemy::HitReactTagChange(const FGameplayTag GameplayTag, int32 NewCount)
{
	bHitReact = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReact ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent()) { // AuraAIController只在Server有效，在Client可能为nullptr
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReact);
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComp->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComp)->AbilityActorInfoSet();

	if (HasAuthority()) {
		this->InitDefaultAttributes();
	}
	OnAscRegistered.Broadcast(AbilitySystemComp);
}

void AAuraEnemy::InitDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitDefaultAttributes(this, CharacterClass, Level, AbilitySystemComp);
}
