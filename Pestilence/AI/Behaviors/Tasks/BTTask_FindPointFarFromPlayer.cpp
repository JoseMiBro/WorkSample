#include "Pestilence/AI/Behaviors/Tasks/BTTask_FindPointFarFromPlayer.h"

#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

UBTTask_FindPointFarFromPlayer::UBTTask_FindPointFarFromPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find point far from Player";
}

EBTNodeResult::Type UBTTask_FindPointFarFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PestilenceAIController->GetBlackboard()->GetValueAsObject(Keys::Player));

	if(!IsValid(PestilenceAIController)) return EBTNodeResult::Succeeded;
	if(!IsValid(PlayerController)) return EBTNodeResult::Succeeded;
	
	const FVector PlayerPosition = PlayerController->GetPawn()->GetActorLocation();
	const FVector PawnPosition = PestilenceAIController->GetPawn()->GetActorLocation();
	const FVector Origin = PawnPosition + (PawnPosition - PlayerPosition).GetSafeNormal() * Distance;

	//DrawDebugSphere(GetWorld(), Origin, Radius, 24, FColor::Cyan, false, 2.f);

	FNavLocation RandomPoint;

	if(const UWorld* World = GetWorld())
	{
		const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		
		if (NavigationSystem->GetRandomPointInNavigableRadius(Origin, Radius, RandomPoint))
		{
			//DrawDebugSphere(GetWorld(), Origin, Radius, 12, FColor::Cyan, false, 2.f);
			PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::RandomPoint, RandomPoint.Location);
		}
		else
		{
			if(NavigationSystem->GetRandomPointInNavigableRadius(PawnPosition, Radius * 2, RandomPoint))
			{
				//DrawDebugSphere(GetWorld(), PawnPosition, Radius * 2, 12, FColor::Cyan, false, 2.f);
				PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::RandomPoint, RandomPoint.Location);
			}
		}
	}
	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindPointFarFromPlayer::GetStaticDescription() const
{
	return FString("Find random point to scape");
}