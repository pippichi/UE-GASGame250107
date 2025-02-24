// QYF Legal Description


#include "UI\WidgetController\OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player\AuraPlayerState.h"
#include "AbilitySystem\Data\LevelUpInfo.h"

void UOverlayWidgetController::BroadcastInitValues()
{
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnXPChangeDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	GetAuraPS()->OnLevelChangeDelegate.AddLambda([this](int32 NewLevel) {
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		});

	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {OnHealthChanged.Broadcast(Data.NewValue);});
	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxHealthChanged.Broadcast(Data.NewValue);});
	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {OnManaChanged.Broadcast(Data.NewValue);});
	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxManaChanged.Broadcast(Data.NewValue);});

	if (GetAuraASC()) {
		if (GetAuraASC()->bStartupAbilitiesGiven) {
			// ASC�Ѿ�Broadcast���ˣ���Ҫ�Լ��ֶ�ִ��
			BroadcastAbilityInfo();
		}
		else {
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
		}
		
		GetAuraASC()->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0) {
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / DeltaLevelRequirement;

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

