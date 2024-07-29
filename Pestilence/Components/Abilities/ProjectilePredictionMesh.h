#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectilePredictionMesh.generated.h"

UCLASS(Blueprintable)
class PESTILENCE_API AProjectilePredictionMesh : public AActor
{
	GENERATED_BODY()

public:	
	AProjectilePredictionMesh();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> DefaultMesh;

	void SetVisible(bool _bVisible);
};
