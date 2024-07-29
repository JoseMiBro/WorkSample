#include "Pestilence/AI/Behaviors/Decorators/DynamicWeightFunctions/InfectedWeightFunction.h"
#include "Pestilence/AI/Behaviors/Decorators/PriorityDecorator.h"

float UInfectedWeightFunction::CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp)
{
	float BaseWeight = Decorator->GetBasePriority();
	float DynamicWeight = BaseWeight;

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp->GetAIOwner()->GetBlackboardComponent();

	EPestilenceStates AIState = EPestilenceStates(Blackboard->GetValueAsEnum(Keys::State));

	if (AIState == EPestilenceStates::Infected)
		return DynamicWeight;
	
	return 0;
}
