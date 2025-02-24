// QYF Legal Description


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player\AuraPlayerState.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	Super::BroadcastInitValues();
	
	check(AttributeInfo);

	// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	// Info.AttributeValue = AS->GetStrength();
	// AttributeInfoSignature.Broadcast(Info);
	for (auto& MapPair: GetAuraAS()->TagToAttributes)
	{
		BroadcastAttributeInfo(MapPair.Key, MapPair.Value());
		// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(MapPair.Key);
		// // Info.AttributeValue = MapPair.Value.Execute().GetNumericValue(AS);
		// Info.AttributeValue = MapPair.Value().GetNumericValue(AS);
		// AttributeInfoSignature.Broadcast(Info);
	}

	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());

}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	check(AttributeInfo);
	for (auto& MapPair: GetAuraAS()->TagToAttributes)
	{
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(MapPair.Value()).AddLambda(
			[this, MapPair](const FOnAttributeChangeData& Data) // 这里为什么使用值捕获而不是引用捕获？因为这是回调函数，真正触发的时候MapPair、AS可能为NULL
			{
				BroadcastAttributeInfo(MapPair.Key, MapPair.Value());
			}
		);
	}
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda([this](int32 Points) {
		AttributePointsChangedDelegate.Broadcast(Points);
		});
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag, const FGameplayAttribute& GameplayAttribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(GameplayTag);
	Info.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoSignature.Broadcast(Info);
}
