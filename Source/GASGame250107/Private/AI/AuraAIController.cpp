// QYF Legal Description


#include "AI/AuraAIController.h"

#include "BehaviorTree\BlackboardComponent.h"
#include "BehaviorTree\BehaviorTreeComponent.h"

AAuraAIController::AAuraAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComp);
}
