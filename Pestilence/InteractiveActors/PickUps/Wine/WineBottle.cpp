#include "Pestilence/InteractiveActors/PickUps/Wine/WineBottle.h"

AWineBottle::AWineBottle()
{
	//Mesh->OnComponentHit.AddDynamic(this, &AWineBottle::OnBottleHit);
}

void AWineBottle::DefaultInteraction(AActor* Actor)
{
	Super::DefaultInteraction(Actor);
}

void AWineBottle::OnBottleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}
