#pragma once

#include "CoreMinimal.h"
#include "Pestilence/InteractiveActors/InteractiveActor.h"
#include "IBWine.generated.h"

UCLASS()
class PESTILENCE_API AIBWine : public AInteractiveActor
{
	GENERATED_BODY()

public:
	AIBWine();

	virtual void DefaultInteraction(AActor* Actor);

	UFUNCTION(BlueprintNativeEvent)
	void OnCorrectPickUp();

	UFUNCTION(BlueprintNativeEvent)
	void OnIncorrectPickUp();

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	FString CorrectPickUpName;
};
