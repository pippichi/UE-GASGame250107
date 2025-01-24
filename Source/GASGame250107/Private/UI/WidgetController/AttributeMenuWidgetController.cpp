// QYF Legal Description


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	Super::BroadcastInitValues();

	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	check(AttributeInfo);

	// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	// Info.AttributeValue = AS->GetStrength();
	// AttributeInfoSignature.Broadcast(Info);
	for (auto& MapPair: AS->TagToAttributes)
	{
		BroadcastAttributeInfo(MapPair.Key, MapPair.Value());
		// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(MapPair.Key);
		// // Info.AttributeValue = MapPair.Value.Execute().GetNumericValue(AS);
		// Info.AttributeValue = MapPair.Value().GetNumericValue(AS);
		// AttributeInfoSignature.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (auto& MapPair: AS->TagToAttributes)
	{
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(MapPair.Value()).AddLambda(
			[this, MapPair](const FOnAttributeChangeData& Data) // 这里为什么使用值捕获而不是引用捕获？因为这是回调函数，真正触发的时候MapPair、AS可能为NULL
			{
				BroadcastAttributeInfo(MapPair.Key, MapPair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag, const FGameplayAttribute& GameplayAttribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(GameplayTag);
	Info.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoSignature.Broadcast(Info);
}
