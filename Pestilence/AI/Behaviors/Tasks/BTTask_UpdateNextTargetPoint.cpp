#include "BTTask_UpdateNextTargetPoint.h"

#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Components/AI/PestilenceAIComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_UpdateNextTargetPoint::UBTTask_UpdateNextTargetPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Update Next TargetPoint");
}

EBTNodeResult::Type UBTTask_UpdateNextTargetPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<AAIController> AIController = Cast<AAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(AIController))
		return EBTNodeResult::Failed;
	
	const TObjectPtr<UPestilenceAIComponent> PestilenceAIComponent = Cast<UPestilenceAIComponent>(AIController->GetPawn()->GetComponentByClass(UPestilenceAIComponent::StaticClass()));

	if (!IsValid(PestilenceAIComponent))
		return EBTNodeResult::Failed;
	
	int32 const TargetPointSize = PestilenceAIComponent->TargetPoints.Num() - 1;
	int32 TargetPointIndex = AIController->GetBlackboardComponent()->GetValueAsInt(Keys::TargetPointIndex);

	if (TargetPointSize < 0)
	{
#if WITH_EDITOR
		const FString ActorName = OwnerComp.GetAIOwner()->GetPawn()->GetActorLabel();
		UE_LOG(LogTemp, Warning, TEXT("Advertencia: El actor %s no tiene TargetPoints."), *ActorName);
#endif
		return EBTNodeResult::Failed;
	}

	if (TargetPointIndex > TargetPointSize)
	{
		TargetPointIndex = 0;
		AIController->GetBlackboardComponent()->SetValueAsInt(Keys::TargetPointIndex, TargetPointIndex);
	}
	ensureMsgf(IsValid(PestilenceAIComponent->TargetPoints[TargetPointIndex]),TEXT("Target Point, is not valid, probably null"));

	if(IsValid(PestilenceAIComponent->TargetPoints[TargetPointIndex]))
	{
		if (PestilenceAIComponent->TargetPoints[TargetPointIndex]->GetPointActive() == false)
		{
			++TargetPointIndex;
			if (TargetPointIndex > TargetPointSize)
				TargetPointIndex = 0;
			
			AIController->GetBlackboardComponent()->SetValueAsInt(Keys::TargetPointIndex, TargetPointIndex);
			
			return EBTNodeResult::Failed;
		}
	}
	
	if(IsValid(PestilenceAIComponent->TargetPoints[TargetPointIndex]))
	{
		AIController->GetBlackboardComponent()->SetValueAsFloat(Keys::WaitTime, PestilenceAIComponent->TargetPoints[TargetPointIndex]->WaitTime);
		//AIController->GetBlackboardComponent()->SetValueAsVector(Keys::Rotate, PestilenceAIComponent->TargetPoints[TargetPointIndex]->Pivot->GetComponentRotation().Euler());
		AIController->GetBlackboardComponent()->SetValueAsVector(Keys::Rotate, PestilenceAIComponent->TargetPoints[TargetPointIndex]->Pivot->GetComponentLocation());
		AIController->GetBlackboardComponent()->SetValueAsVector(Keys::TargetPointPosition, PestilenceAIComponent->TargetPoints[TargetPointIndex]->GetActorLocation());

		++TargetPointIndex;
		if (TargetPointIndex > TargetPointSize)
			TargetPointIndex = 0;
		
		AIController->GetBlackboardComponent()->SetValueAsInt(Keys::TargetPointIndex, TargetPointIndex);
	}

	return EBTNodeResult::Succeeded;
}

FString UBTTask_UpdateNextTargetPoint::GetStaticDescription() const
{
	return TEXT("Update the next target point in my pestilence character");
}