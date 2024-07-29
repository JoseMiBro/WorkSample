#pragma once

#include "CoreMinimal.h"
#include "Pestilence/InteractiveActors/IActors/PickUpBase.h"
#include "WineBottle.generated.h"

UCLASS()
class PESTILENCE_API AWineBottle : public APickUpBase
{
	GENERATED_BODY()

public:

	AWineBottle();

	virtual void DefaultInteraction(AActor* Actor) override;

	void OnBottleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
