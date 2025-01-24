// QYF Legal Description

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAttributeMenuWidgetController;
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
};
