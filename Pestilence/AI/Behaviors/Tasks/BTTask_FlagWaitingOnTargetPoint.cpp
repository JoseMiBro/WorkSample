// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FlagWaitingOnTargetPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Animation/PestilenceCharacter/PestilenceCharacterAnimInstance.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/AI/PestilenceAIComponent.h"

EBTNodeResult::Type UBTTask_FlagWaitingOnTargetPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	const TObjectPtr<APestilenceCharacter> AIPawn =Cast<APestilenceCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	TObjectPtr<UPestilenceCharacterAnimInstance> AnimInstance = Cast<UPestilenceCharacterAnimInstance>(AIPawn->GetMesh()->GetAnimInstance());

	if(!IsValid(AnimInstance)) return EBTNodeResult::Failed;
	AnimInstance->bIsWaitingOnPatrolPoint = IsWaitingOnTargetPoint;

	if(IsWaitingOnTargetPoint)
	{
		const TObjectPtr<UPestilenceAIComponent> AIComponent = AIPawn->FindComponentByClass<UPestilenceAIComponent>();
		if(!IsValid(AIComponent)) return EBTNodeResult::Failed;

		int TargetPointIdx = OwnerComp.GetBlackboardComponent()->GetValueAsInt(Keys::TargetPointIndex) - 1;

		if (TargetPointIdx < 0 )
			TargetPointIdx = (AIComponent->TargetPoints.Num()-1);

		ensureMsgf(IsValid(AIComponent->TargetPoints[TargetPointIdx]),TEXT("Target Point, is not valid, probably null"));
		
		if(IsValid(AIComponent->TargetPoints[TargetPointIdx]))
		{
			if(AIComponent->TargetPoints[TargetPointIdx]->bPointActive)
			{
				if(AIComponent->TargetPoints[TargetPointIdx]->WaitingAnimation)
				{
					AnimInstance->WaitingAnimation = AIComponent->TargetPoints[TargetPointIdx]->WaitingAnimation;
				}
				else
				{
					AnimInstance->WaitingAnimation = AnimInstance->DefaultWaitingAnimation;
				}
			}
		}
	}
	//else
	//{
	//	AnimInstance->WaitingAnimation = AnimInstance->DefaultWaitingAnimation;
	//}
		
	return EBTNodeResult::Succeeded;
}
