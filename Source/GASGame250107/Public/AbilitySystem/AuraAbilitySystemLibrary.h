// QYF Legal Description

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class GASGAME250107_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "AuraAttributeSystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverLayWidgetController(const UObject* WorldContextObj);

	UFUNCTION(BlueprintPure, Category = "AuraAttributeSystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObj);

	UFUNCTION(BlueprintCallable, Category = "AuraAttributeSystemLibrary|CharacterClassDefaults")
	static void InitDefaultAttributes(const UObject* WorldContextObj, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAttributeSystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObj, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAttributeSystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObj);
};
