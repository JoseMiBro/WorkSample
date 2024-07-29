#include "Pestilence/AI/Behaviors/Tasks/BTTask_Assassinate.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Pestilence/Components/Possession/RagdollState.h"

UBTTask_Assassinate::UBTTask_Assassinate(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Assassinate";
}

EBTNodeResult::Type UBTTask_Assassinate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(PestilenceAIController))
		return EBTNodeResult::Failed;
	
	const TObjectPtr<APestilenceCharacter> PawnPestilenceCharacter = Cast<APestilenceCharacter>(PestilenceAIController->GetBlackboard()->GetValueAsObject(Keys::PawnActor));

	if (!IsValid(PawnPestilenceCharacter))
		return EBTNodeResult::Failed;
	
	UPestilencePossessionComponent* PossessionComponentPawn = PawnPestilenceCharacter->GetPestilencePossessionComponent();
	const TObjectPtr<APestilenceAIController> PawnController = Cast<APestilenceAIController>(PawnPestilenceCharacter->GetController());
	
	if (!IsValid(PossessionComponentPawn) || !IsValid(PawnController))
		return EBTNodeResult::Failed;

	PawnController->SetCurrentState(EPestilenceStates::Dead);
	PossessionComponentPawn->SwitchState(PossessionComponentPawn->GetRagdollState());
	return EBTNodeResult::Succeeded;
}

FString UBTTask_Assassinate::GetStaticDescription() const
{
	return FString("Assassinate Infected Pawn");
}
