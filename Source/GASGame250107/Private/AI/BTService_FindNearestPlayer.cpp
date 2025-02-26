// QYF Legal Description


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "Kismet\GameplayStatics.h"
#include "BehaviorTree\BTFunctionLibrary.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, *AIOwner.GetName());
	//GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, *ActorOwner.GetName());

	APawn* OwningPawn = AIOwner->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (auto Actor : ActorsWithTag) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, *Actor->GetName()); 由于是for循环，因此第一个参数需为-1，以保证日志不会被刷掉
		if (IsValid(Actor) && IsValid(OwningPawn)) {
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance) {
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	if (ClosestActor != nullptr) {
		UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
		UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToFollowSelector, ClosestDistance);
	}
}
