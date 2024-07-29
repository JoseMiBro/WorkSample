#include "Pestilence/AI/Behaviors/Tasks/BTTask_FindNearestGuard.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/AI/PestilenceAIComponent.h"

UBTTask_FindNearestGuard::UBTTask_FindNearestGuard(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Guard";
}

EBTNodeResult::Type UBTTask_FindNearestGuard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	const TObjectPtr<UPestilenceAIComponent> PestilenceAIComponent = Cast<UPestilenceAIComponent>(PestilenceAIController->GetPawn()->GetComponentByClass(UPestilenceAIComponent::StaticClass()));
	
	if (PestilenceAIController == nullptr || PestilenceAIComponent == nullptr)
		return EBTNodeResult::Failed;
	

	//PestilenceAIComponent->PestilenceCharacters;

	for (APestilenceCharacter* Actor : PestilenceAIComponent->PestilenceCharacters)
	{
		if(!IsValid(Actor)) continue;
		if (Actor->GetControllerAI()->GetCurrentState() != EPestilenceStates::Chase && Actor->GetControllerAI()->GetCurrentState() != EPestilenceStates::Stun)
		{
			PestilenceAIController->GetBlackboard()->SetValueAsObject(Keys::GuardActor, Actor);
			break;
		}
	}
	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindNearestGuard::GetStaticDescription() const
{
	return Super::GetStaticDescription();
}