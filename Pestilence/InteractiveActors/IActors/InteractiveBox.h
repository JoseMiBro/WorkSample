#pragma once

#include "CoreMinimal.h"
#include "Pestilence/InteractiveActors/InteractiveActor.h"
#include "InteractiveBox.generated.h"

UCLASS()
class PESTILENCE_API AInteractiveBox : public AInteractiveActor
{
	GENERATED_BODY()

public:

	virtual void DefaultInteraction(AActor* Actor);

	UFUNCTION(BlueprintNativeEvent)
	void OnInteract();	
};
