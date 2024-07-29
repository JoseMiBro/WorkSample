#include "Pestilence/Components/AI/Reaction/PawnReactionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/AI/Enums/EPestilenceTypeCharacter.h"

UPawnReactionComponent::UPawnReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Type = ECharacterType::Pawn;
}

void UPawnReactionComponent::React_Implementation(APestilenceAIController* PestilenceAIController,
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
			if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Infected)
			{
				if (OwnerAIController->GetCurrentState() != EPestilenceStates::Escape)
				{
					OwnerAIController->GetBlackboard()->SetValueAsObject(Keys::PawnActor, PestilenceAIController->GetPawn());
					OwnerAIController->GetBlackboard()->SetValueAsBool(Keys::SomeOneInfected, true);
					OwnerAIController->Attention = 90.f;
				}
			}
		} break;
		
	case EPestilenceTypeCharacter::Foreman:
		{
			if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Advise)
			{
				if (OwnerAIController->GetCurrentState() != EPestilenceStates::Alert)
				{
					OwnerAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, PestilenceAIController->GetPawn()->GetActorLocation());
					OwnerAIController->Attention = 60.f;
				}
			}
		} break;
		
	case  EPestilenceTypeCharacter::Guard:
		{
			if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Chase)
			{
				if (OwnerAIController->GetCurrentState() != EPestilenceStates::Alert)
				{
					OwnerAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, PestilenceAIController->GetPawn()->GetActorLocation());
					OwnerAIController->Attention = 60.f;
				}
			}
		} break;
	default: break;;
	}
}