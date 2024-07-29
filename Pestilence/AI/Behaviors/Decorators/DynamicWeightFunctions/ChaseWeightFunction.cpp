#include "ChaseWeightFunction.h"

#include "Pestilence/AI/Behaviors/Decorators/PriorityDecorator.h"

float UChaseWeightFunction::CalculateDynamicWeight(const UPriorityDecorator* Decorator,
                                                   UBehaviorTreeComponent* OwnerComp)
{
	float BaseWeight = Decorator->GetBasePriority();
	float DynamicWeight = BaseWeight;

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp->GetAIOwner()->GetBlackboardComponent();
	TObjectPtr<APestilenceAIController> Controller = Cast<APestilenceAIController>(OwnerComp->GetAIOwner());

	float Attention = Blackboard->GetValueAsFloat(Keys::Alert);

	if(Attention >= Controller->ChaseInFrontier)/*Controller->BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(
		Controller->StateToString(EPestilenceStates::Chase),"")->InStateAttentionFrontier)*/
		return DynamicWeight;
	
	return 0;
}
