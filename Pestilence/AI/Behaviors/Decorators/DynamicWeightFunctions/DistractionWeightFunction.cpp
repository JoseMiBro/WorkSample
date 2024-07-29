// Fill out your copyright notice in the Description page of Project Settings.


#include "DistractionWeightFunction.h"

#include "Pestilence/AI/Behaviors/Decorators/PriorityDecorator.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

float UDistractionWeightFunction::CalculateDynamicWeight(const UPriorityDecorator* Decorator,
                                                         UBehaviorTreeComponent* OwnerComp)
{
	float BaseWeight = Decorator->GetBasePriority();
	float DynamicWeight = BaseWeight;

	TObjectPtr<APestilenceAIController> Controller = Cast<APestilenceAIController>(OwnerComp->GetAIOwner());
	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp->GetAIOwner()->GetBlackboardComponent();

	bool bPawnInfected = Blackboard->GetValueAsBool(Keys::SomeOneInfected);
	float Attention = Blackboard->GetValueAsFloat(Keys::Alert);
	APestilenceCharacter* Pawn = Cast<APestilenceCharacter>(Blackboard->GetValueAsObject(Keys::PawnActor));

	if (IsValid(Pawn))
	{
		if(bPawnInfected == true)
			if (Attention <= Controller->ChaseInFrontier/*Controller->BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(
			Controller->StateToString(EPestilenceStates::Chase),"")->InStateAttentionFrontier*/)
				if (Pawn->GetControllerAI()->GetCurrentState() != EPestilenceStates::Dead)
					return DynamicWeight;
	}

	return 0;
}
