#include "Pestilence/AI/Behaviors/Decorators/DynamicWeightFunctions/WatchdogWeightFunction.h"

#include "Pestilence/AI/Behaviors/Decorators/PriorityDecorator.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

float UWatchdogWeightFunction::CalculateDynamicWeight(const UPriorityDecorator* Decorator,
                                                      UBehaviorTreeComponent* OwnerComp)
{
	float BaseWeight = Decorator->GetBasePriority();
	float DynamicWeight = BaseWeight;

	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp->GetAIOwner()->GetBlackboardComponent();
	
	bool Warned = Blackboard->GetValueAsBool(Keys::WarnedGuard);
	APestilenceCharacter* Guard = Cast<APestilenceCharacter>(Blackboard->GetValueAsObject(Keys::GuardActor));

	if (Warned == true)
		if (IsValid(Guard))
			if (Guard->GetControllerAI()->GetCurrentState() == EPestilenceStates::Chase ||
				Guard->GetControllerAI()->GetCurrentState() == EPestilenceStates::Alert)
				return DynamicWeight;

	
	return 0;
}
