// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/AI/Behaviors/Decorators/DynamicWeightFunctions/EscapeWeightFunction.h"
#include "EscapeWeightFunction.h"
#include "Pestilence/AI/Behaviors/Decorators/PriorityDecorator.h"

float UEscapeWeightFunction::CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp)
{
	float BaseWeight = Decorator->GetBasePriority();
	float DynamicWeight = BaseWeight;

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp->GetAIOwner()->GetBlackboardComponent();
	const TObjectPtr<APestilenceAIController> Controller = Cast<APestilenceAIController>(OwnerComp->GetAIOwner());
	
	float Attention = Blackboard->GetValueAsFloat(Keys::Alert);
	
	if (Attention >= Controller->EscapeInFrontier)	/*Controller->BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(
	Controller->StateToString(EPestilenceStates::Escape),"")->InStateAttentionFrontier)*/
		return DynamicWeight;

	return 0;
}
