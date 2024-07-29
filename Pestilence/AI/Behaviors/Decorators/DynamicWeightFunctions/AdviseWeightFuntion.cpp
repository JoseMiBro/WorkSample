#include "Pestilence/AI/Behaviors/Decorators/DynamicWeightFunctions/AdviseWeightFuntion.h"

#include "Pestilence/AI/Behaviors/Decorators/PriorityDecorator.h"

float UAdviseWeightFuntion::CalculateDynamicWeight(const UPriorityDecorator* Decorator,
                                                   UBehaviorTreeComponent* OwnerComp)
{
	float BaseWeight = Decorator->GetBasePriority();
	float DynamicWeight = BaseWeight;

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp->GetAIOwner()->GetBlackboardComponent();
	const TObjectPtr<APestilenceAIController> Controller = Cast<APestilenceAIController>(OwnerComp->GetAIOwner());

	if(!IsValid(Blackboard)) return 0;
	
	float Attention = Blackboard->GetValueAsFloat(Keys::Alert);
	if (Attention >= Controller->AdviseInFrontier)
		//Controller->BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(
	//Controller->StateToString(EPestilenceStates::Advise),"")->InStateAttentionFrontier)
		return DynamicWeight;
	
	return 0;
}
