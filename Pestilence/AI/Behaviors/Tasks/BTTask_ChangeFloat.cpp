// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeFloat.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"

EBTNodeResult::Type UBTTask_ChangeFloat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const FName BlackboardKeyName = BlackboardKey.SelectedKeyName;

	const FString StringBBKey = BlackboardKeyName.ToString();

	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	if(PestilenceAIController == nullptr)
		return EBTNodeResult::Failed;

	const float bKeyValue = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BlackboardKeyName);
	
	if (bKeyValue == NewValue)
	{
		return EBTNodeResult::Succeeded;
	}

	PestilenceAIController->GetBlackboard()->SetValueAsFloat(BlackboardKeyName, NewValue);

	return EBTNodeResult::Succeeded;
}
