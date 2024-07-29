// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertWeightFunction.h"

#include "../PriorityDecorator.h"

float UAlertWeightFunction::CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp)
{
	float BaseWeight = Decorator->GetBasePriority();
	float DynamicWeight = BaseWeight;

	const TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp->GetAIOwner()->GetBlackboardComponent();
	const TObjectPtr<APestilenceAIController> Controller = Cast<APestilenceAIController>(OwnerComp->GetAIOwner());
	const float Attention = Blackboard->GetValueAsFloat(Keys::Alert);
	const EPestilenceStates State = static_cast<EPestilenceStates>(Blackboard->GetValueAsEnum(Keys::State));

	if( (State == EPestilenceStates::Patrol && Attention >= Controller->AlertInFrontier)
			/*Attention >= Controller->BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(
		Controller->StateToString(EPestilenceStates::Alert),"")->InStateAttentionFrontier)*/
		||
		(State == EPestilenceStates::Chase && Attention <= Controller->AlertOutFrontier)
			/*Attention <= Controller->BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(
		Controller->StateToString(EPestilenceStates::Alert),"")->OutStateAttentionFrontier)*/
		||
		(State != EPestilenceStates::Patrol && Attention > Controller->PatrolOutFrontier)
			/*Attention > Controller->BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(
		Controller->StateToString(EPestilenceStates::Patrol),"")->OutStateAttentionFrontier))*/
		)
			return DynamicWeight;

	return 0;
}
