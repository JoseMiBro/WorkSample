#pragma once

#include "CoreMinimal.h"
#include "Pestilence/InteractiveActors/InteractiveActor.h"
#include "PickUpBase.generated.h"

UCLASS()
class PESTILENCE_API APickUpBase : public AInteractiveActor
{
	GENERATED_BODY()

public:
	APickUpBase();

	UPROPERTY(EditAnywhere, Category = "Pickeable | Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	USceneComponent* IActorHolding;

	UPROPERTY()
	APestilenceCharacter* CurrentPicker;

	UPROPERTY()
	bool bPhysics = false; //Checkear que hace esto en la logica
	
	UPROPERTY(BlueprintReadOnly)
	bool bHolding;
	
	// Makes the Pickeable throwable instead of just dropping it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickeable | Throw?")
	bool bThrowable;

	// Force applied on throw, in centiNewtons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickeable | Throw?")
	float ThrowForce = 100000.f;

	virtual void DefaultInteraction(AActor* Actor) override;

};
