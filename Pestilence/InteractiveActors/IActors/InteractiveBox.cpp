#include "Pestilence/InteractiveActors/IActors/InteractiveBox.h"

void AInteractiveBox::DefaultInteraction(AActor* Actor)
{
	if (CurrentInteractor->CurrentPickUp)
	{
		
		//CurrenInteractor->CurrentPickUp->DefaultInteraction(Actor);

		OnInteract();
	}
	
}
void AInteractiveBox::OnInteract_Implementation()
{
	
}
