#include "Pestilence/InteractiveActors/PickUps/Wine/IBWine.h"

AIBWine::AIBWine()
{
	CorrectPickUpName = TEXT("Expensive");
}

void AIBWine::DefaultInteraction(AActor* Actor)
{
	if (CurrentInteractor->CurrentPickUp && CurrentInteractor->CurrentPickUp->Name == CorrectPickUpName)
	{
		CurrentInteractor->CurrentPickUp->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		CurrentInteractor->CurrentPickUp->Destroy();

		CurrentInteractor->CurrentPickUp = nullptr;
		
		OnCorrectPickUp();
	}
	else
	{
		OnIncorrectPickUp();
	}
}

void AIBWine::OnCorrectPickUp_Implementation()
{
}

void AIBWine::OnIncorrectPickUp_Implementation()
{
}
