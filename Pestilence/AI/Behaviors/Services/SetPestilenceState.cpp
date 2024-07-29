// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/AI/Behaviors/Services/SetPestilenceState.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"

USetPestilenceState::USetPestilenceState()
{
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
	NodeName = TEXT("Set Pestilence State");
}

void USetPestilenceState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	const TObjectPtr<APestilenceAIController> OwnerController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());
	TObjectPtr<UBlackboardComponent> BlackboardComponent = OwnerComp.GetBlackboardComponent();
	OwnerController->SetCurrentState(NewState);
	//BlackboardComponent->SetValueAsEnum(Keys::LastState, GetTypeHash(OwnerController->GetCurrentState()));
	//OwnerController->CurrentState = NewState;
	//BlackboardComponent->SetValueAsEnum(Keys::State, GetTypeHash(NewState));

	const EPestilenceStates AuxLastState = static_cast<EPestilenceStates>(BlackboardComponent->GetValueAsEnum(Keys::LastState));
	const EPestilenceStates AuxState = static_cast<EPestilenceStates>(BlackboardComponent->GetValueAsEnum(Keys::State));
	if(AuxLastState != AuxState  || AuxState == EPestilenceStates::Infected)
	{
		OwnerController->ChangeOfStateReported = false;
	}
}

