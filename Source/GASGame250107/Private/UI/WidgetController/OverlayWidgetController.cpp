// QYF Legal Description


#include "UI\WidgetController\OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem\Data\AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitValues()
{
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {OnHealthChanged.Broadcast(Data.NewValue);});
	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxHealthChanged.Broadcast(Data.NewValue);});
	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {OnManaChanged.Broadcast(Data.NewValue);});
	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxManaChanged.Broadcast(Data.NewValue);});

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComp)) {
		if (AuraASC->bStartupAbilitiesGiven) {
			// ASC已经Broadcast过了，需要自己手动执行
			OnInitializeStartupAbilities(AuraASC);
		}
		else {
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
		}
		
		AuraASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& TagContainer)
			{
				for (const FGameplayTag& Tag : TagContainer)
				{
					// For example,say that Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return true, "Message".MatchesTag("Message.HealthPotion") will return false
					const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						if (const FUIWidgetRow* UIWidgetRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
						{
							MessageWidgetRowSignature.Broadcast(*UIWidgetRow);
						}
					}
				}
			}
		);
	}
	
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
{
	// Get information about all given abilities, look up their Ability Info, and broadcast it to widgets

	//FForEachAbility BroadcastDelegate;
	//BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec) {
	//	// a way to figure out the gameplay tag for a given ability spec
	//	const FGameplayTag Tag = UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec);
	//	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(Tag);
	//	Info.InputTag = UAuraAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
	//	AbilityInfoDelegable.Broadcast(Info);
	//});
	//AuraASC->ForEachAbility(BroadcastDelegate);
	AuraASC->ForEachAbility1([this](const FGameplayAbilitySpec& AbilitySpec) { // 此为直接用c++原生函数指针代替上面BroadcastDelegate的写法
		// a way to figure out the gameplay tag for a given ability spec
		const FGameplayTag Tag = UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec);
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(Tag);
		Info.InputTag = UAuraAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegable.Broadcast(Info);
		return true;
	});
}

