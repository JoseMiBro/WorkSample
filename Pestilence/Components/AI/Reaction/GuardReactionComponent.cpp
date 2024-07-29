#include "Pestilence/Components/AI/Reaction/GuardReactionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/AI/Enums/EPestilenceTypeCharacter.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

UGuardReactionComponent::UGuardReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Type = ECharacterType::Guard;
}

void UGuardReactionComponent::React_Implementation(APestilenceAIController* PestilenceAIController,
                                                   EPestilenceTypeCharacter TypeCharacter)
{
	Super::React_Implementation(PestilenceAIController, TypeCharacter);
	
	if(!IsValid(OwnerAIController) || !IsValid(PestilenceAIController) || !IsValid(OwnerAIController->GetBlackboard()) || !IsValid(PestilenceAIController->GetPawn()))
		return;

	//const FString CurrentStateText = UEnum::GetValueAsString(PestilenceAIController->GetCurrentState());
	
	switch (TypeCharacter)
	{
	case EPestilenceTypeCharacter::Pawn:
		{
			if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Infected && OwnerAIController->GetCurrentState() != EPestilenceStates::Distracted)
			{
				OwnerAIController->GetBlackboard()->SetValueAsBool(Keys::SomeOneInfected, true);
				OwnerAIController->GetBlackboard()->SetValueAsObject(Keys::PawnActor, PestilenceAIController->GetPawn());
			}
			else if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Escape && OwnerAIController->GetCurrentState() != EPestilenceStates::Distracted && OwnerAIController->GetCurrentState() != EPestilenceStates::Alert)
			{
				OwnerAIController->GetBlackboard()->SetValueAsVector(Keys::PestilenceCharacterLKP, PestilenceAIController->GetPawn()->GetActorLocation());
				OwnerAIController->GetBlackboard()->SetValueAsBool(Keys::SomeOneScaping, true); //TODO: Check tree if works correctly 
				OwnerAIController->Attention = 60.f;
			}
		} break;
		
	case EPestilenceTypeCharacter::Foreman:
		{
			//Here, just in case
		} break;
		
	case  EPestilenceTypeCharacter::Guard:
		{
			if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Chase)
			{
				if (OwnerAIController->GetCurrentState() == EPestilenceStates::Patrol || OwnerAIController->GetCurrentState() == EPestilenceStates::Doorman)
				{
					if (IsValid(OwnerAIController->GetBlackboard()))
					{
						if(!OwnerAIController->GetBlackboard()->GetValueAsBool(Keys::Witness))
							OwnerAIController->OnWitnessChange.Broadcast(true);
						OwnerAIController->GetBlackboard()->SetValueAsBool(Keys::Witness, true);
						OwnerAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, PestilenceAIController->GetPawn()->GetActorLocation());
						OwnerAIController->Attention = 60.f;
					}
				}
			}
		} break;
	default: break;;
	}
}
