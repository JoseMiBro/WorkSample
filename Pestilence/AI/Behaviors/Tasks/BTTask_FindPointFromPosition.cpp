#include "Pestilence/AI/Behaviors/Tasks/BTTask_FindPointFromPosition.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"

UBTTask_FindPointFromPosition::UBTTask_FindPointFromPosition(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "FindPointFromPosition";
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointFromPosition, BlackboardKey));
	BlackboardResultKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointFromPosition, BlackboardResultKey));
}

EBTNodeResult::Type UBTTask_FindPointFromPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const FName BlackboardKeyName = BlackboardKey.SelectedKeyName;
	const FVector Position = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKeyName);
	const FName BlackboardResultName = BlackboardResultKey.SelectedKeyName;

	FVector LastPlayerMovementDirection = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Keys::PlayerMovementDirection);
	
	FNavLocation WanderingPoint;

	if(UWorld* World = GetWorld())
	{
		const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(World);

		if(NavigationSystem->GetRandomPointInNavigableRadius(Position + (LastPlayerMovementDirection * DistancePoint), Radius, WanderingPoint))
		{
			//DrawDebugSphere(World, Position + (LastPlayerMovementDirection * DistancePoint), Radius, 12, FColor::Green, false, 3.f);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardResultName, WanderingPoint.Location);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(Keys::LastKnownPoint,Position + (LastPlayerMovementDirection * DistancePoint));
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

FString UBTTask_FindPointFromPosition::GetStaticDescription() const
{
	return FString("Find a navigable point from a position");
}