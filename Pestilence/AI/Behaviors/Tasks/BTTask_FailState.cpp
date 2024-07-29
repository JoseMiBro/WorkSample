#include "Pestilence/AI/Behaviors/Tasks/BTTask_FailState.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/FailState/FailStateComponent.h"

UBTTask_FailState::UBTTask_FailState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Fail State";
}

EBTNodeResult::Type UBTTask_FailState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bPlayerKilled)
		return EBTNodeResult::Succeeded;
	
	const APestilenceAIController* PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(PestilenceAIController) || !PestilenceAIController->GetBlackboard())
		return EBTNodeResult::Failed;

	TObjectPtr<APestilenceCharacter> PestilenceCharacter = Cast<APestilenceCharacter>(PestilenceAIController->GetPawn());

	if (!IsValid(PestilenceCharacter))
		return EBTNodeResult::Failed;
	
	const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PestilenceAIController->GetBlackboard()->GetValueAsObject(Keys::Player));
	const TObjectPtr<APestilenceCharacter> PlayerCharacter = Cast<APestilenceCharacter>(PlayerController->GetPawn());
	
	PlayerCharacter->FailStateComponent->PlayerDieResponse(PestilenceCharacter); //bKillPlayer
	
	bPlayerKilled = true;
	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_FailState::GetStaticDescription() const
{
	return FString("Player Fail State");
}
