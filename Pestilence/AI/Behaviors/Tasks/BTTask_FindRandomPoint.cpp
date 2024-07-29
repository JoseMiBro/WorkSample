#include "Pestilence/AI/Behaviors/Tasks/BTTask_FindRandomPoint.h"

#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"

UBTTask_FindRandomPoint::UBTTask_FindRandomPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Point";
}

EBTNodeResult::Type UBTTask_FindRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	if (PestilenceAIController == nullptr)
		return EBTNodeResult::Failed;

	const FVector PawnPosition = PestilenceAIController->GetPawn()->GetActorLocation();

	const UWorld* World = GetWorld();
	FNavLocation RandomPoint;

	if (World)
	{
		const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavigationSystem->GetRandomPointInNavigableRadius(PawnPosition, Radius, RandomPoint))
		{
			//DrawDebugSphere(GetWorld(), PawnPosition, Radius, 12, FColor::Cyan, false, 2.f);
			PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::RandomPoint, RandomPoint.Location);
		}
	}
	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindRandomPoint::GetStaticDescription() const
{
	return FString();
}