// QYF Legal Description

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class GASGAME250107_API AAuraAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAuraAIController();
	
protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;
};
