// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateToFaceBBEntryCustom.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTTask_RotateToFaceBBEntryCustom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		FRotator PawnRot = UKismetMathLibrary::FindLookAtRotation(
			OwnerComp.GetAIOwner()->GetPawn()->GetActorRotation().Vector(),
			KeyValue
		);

		if(!PawnRot.IsNearlyZero())
			return Super::ExecuteTask(OwnerComp, NodeMemory);
	
	}
	return EBTNodeResult::Succeeded;
}
