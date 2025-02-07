// QYF Legal Description

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASGAME250107_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bBlockHit, bool bCriticalHit);
};
