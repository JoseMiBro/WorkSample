// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/AI/Behaviors/Decorators/DynamicWeightFunctions/StunWeightFunction.h"
#include "Pestilence/AI/Behaviors/Decorators/PriorityDecorator.h"

float UStunWeightFunction::CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp)
{
	float BaseWeight = Decorator->GetBasePriority();
	float DynamicWeight = BaseWeight;

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp->GetAIOwner()->GetBlackboardComponent();

	EPestilenceStates AIState = EPestilenceStates(Blackboard->GetValueAsEnum(Keys::State));
	
	if (AIState == EPestilenceStates::Stun)
		return DynamicWeight;

	return 0;
}
