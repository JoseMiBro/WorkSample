#include "Pestilence/Components/AI/Reaction/ForemanReactionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/AI/Enums/EPestilenceTypeCharacter.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

UForemanReactionComponent::UForemanReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Type = ECharacterType::Foreman;
}

void UForemanReactionComponent::React_Implementation(APestilenceAIController* PestilenceAIController, EPestilenceTypeCharacter TypeCharacter)
{
	Super::React_Implementation(PestilenceAIController, TypeCharacter);

	if (OwnerAIController == nullptr || PestilenceAIController == nullptr)
		return;

	switch (TypeCharacter)
	{
	case EPestilenceTypeCharacter::Pawn:
	{
		const FString CurrentStateText = UEnum::GetValueAsString(PestilenceAIController->GetCurrentState());

		if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Escape)
		{
			if (OwnerAIController->Attention < 60) 
			{
				if (IsValid(OwnerAIController->GetBlackboard()))
					if (IsValid(PestilenceAIController))
					{
						if(IsValid(PestilenceAIController->GetPawn()))
						{
							OwnerAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, PestilenceAIController->GetPawn()->GetActorLocation());
							OwnerAIController->Attention = 60.f;
						}
					}
			}
		}
		else if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Infected)
		{
			if (OwnerAIController->Attention < 95) 
			{
				if (IsValid(OwnerAIController->GetBlackboard()))
					if (IsValid(PestilenceAIController))
					{
						OwnerAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, PestilenceAIController->GetPawn()->GetActorLocation());
						OwnerAIController->Attention = 95.f;
					}
			}
		}
	} break;
	case EPestilenceTypeCharacter::Foreman:
	{
		const FString CurrentStateText = UEnum::GetValueAsString(PestilenceAIController->GetCurrentState());

		if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Advise)
		{
			if (OwnerAIController->Attention < 60)
			{
				if (IsValid(OwnerAIController->GetBlackboard()))
					if (IsValid(PestilenceAIController))
					{
						OwnerAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, PestilenceAIController->GetPawn()->GetActorLocation());
						OwnerAIController->Attention = 60.f;
					}
			}
		}
	} break;
	case  EPestilenceTypeCharacter::Guard:
	{
		const FString CurrentStateText = UEnum::GetValueAsString(PestilenceAIController->GetCurrentState());
			
		if (PestilenceAIController->GetCurrentState() == EPestilenceStates::Alert || PestilenceAIController->GetCurrentState() == EPestilenceStates::Chase)
		{
			if (OwnerAIController->Attention < 60) 
			{
				if (IsValid(OwnerAIController->GetBlackboard()))
					if (IsValid(PestilenceAIController))
					{
						OwnerAIController->GetBlackboard()->SetValueAsVector(Keys::GuardLastKnownPoint, PestilenceAIController->GetPawn()->GetActorLocation());
						OwnerAIController->Attention = 60.f;
					}
			}
		}
	} break;
	default: break;;
	}
}