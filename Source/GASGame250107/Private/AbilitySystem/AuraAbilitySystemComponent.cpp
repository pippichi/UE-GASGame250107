// QYF Legal Description


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

	// const FString Attributes_Secondary_Armor_Str = FAuraGameplayTags::Get().Attributes_Secondary_Armor.ToString();
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Native tags: %s"), *Attributes_Secondary_Armor_Str));
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	// GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer); 

	EffectAssetTags.Broadcast(TagContainer);
	
	// for (const FGameplayTag &Tag : TagContainer)
	// {
	// 	const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
	// 	GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, Msg);
	// }
}
