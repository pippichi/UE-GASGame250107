// QYF Legal Description


#include "UI\WidgetController\AuraWidgetController.h"

#include "Player\AuraPlayerController.h"
#include "Player\AuraPlayerState.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"
#include "AbilitySystem\AuraAttributeSet.h"
#include "AbilitySystem\Data\AbilityInfo.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComp = WidgetControllerParams.AbilitySystemComp;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}

void UAuraWidgetController::BroadcastAbilityInfo()
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
	GetAuraASC()->ForEachAbility1([this](const FGameplayAbilitySpec& AbilitySpec) { // 此为直接用c++原生函数指针代替上面BroadcastDelegate的写法
		// a way to figure out the gameplay tag for a given ability spec
		const FGameplayTag Tag = UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec);
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(Tag);
		Info.InputTag = UAuraAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = UAuraAbilitySystemComponent::GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegable.Broadcast(Info);
		return true;
	});
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr) {
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComp);
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
