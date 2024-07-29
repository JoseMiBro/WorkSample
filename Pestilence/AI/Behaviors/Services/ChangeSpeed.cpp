#include "ChangeSpeed.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

UChangeSpeed::UChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	const TObjectPtr<APestilenceCharacter> PestilenceCharacter = Cast<APestilenceCharacter>(PestilenceAIController->GetPawn());

	PestilenceCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

FString UChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change actor speed");
}