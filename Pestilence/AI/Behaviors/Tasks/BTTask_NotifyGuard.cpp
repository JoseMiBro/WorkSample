#include "Pestilence/AI/Behaviors/Tasks/BTTask_NotifyGuard.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

UBTTask_NotifyGuard::UBTTask_NotifyGuard(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Notify Guard";
}

EBTNodeResult::Type UBTTask_NotifyGuard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	const TObjectPtr<APestilenceCharacter> OtherActor = Cast<APestilenceCharacter>(PestilenceAIController->GetBlackboard()->GetValueAsObject(Keys::GuardActor));
	
	if(PestilenceAIController == nullptr || OtherActor == nullptr)
		return EBTNodeResult::Failed;

	const TObjectPtr<APestilenceAIController> OtherAIController = Cast<APestilenceAIController>(OtherActor->GetController());

	if(OtherAIController == nullptr)
		return EBTNodeResult::Failed;

	const TObjectPtr<APestilenceCharacter> Owner = Cast<APestilenceCharacter>(PestilenceAIController->GetPawn());

	OtherAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, PestilenceAIController->GetBlackboard()->GetValueAsVector(Keys::LastKnownPoint));
	OtherAIController->GetBlackboard()->SetValueAsObject(Keys::ForemanActor, Owner);
	OtherAIController->GetBlackboard()->SetValueAsBool(Keys::Warned, true);
	if(!OtherAIController->GetBlackboard()->GetValueAsBool(Keys::Witness))
		OtherAIController->OnWitnessChange.Broadcast(true);
	OtherAIController->GetBlackboard()->SetValueAsBool(Keys::Witness, true);
	
	PestilenceAIController->GetBlackboard()->SetValueAsBool(Keys::WarnedGuard, true);
	OtherActor->GetControllerAI()->Attention = 70.f;
	return EBTNodeResult::Succeeded;
}

FString UBTTask_NotifyGuard::GetStaticDescription() const
{
	return FString("The Foreman Notifies The LKP Of The Player");
}