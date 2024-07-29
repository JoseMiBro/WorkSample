#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "InteractiveActor.generated.h"

UCLASS(Blueprintable)
class PESTILENCE_API AInteractiveActor : public AActor
{
	GENERATED_BODY()

public:	
	const FString feedBackText = "Press F";

public:	
	AInteractiveActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere)
	//UStaticMeshComponent* DefaultMesh;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APestilenceCharacter> CurrentInteractor;

	UFUNCTION(BlueprintCallable, Category = "Interactive Actor")
	virtual void DefaultInteraction(AActor* Actor);

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	FString Name;
};
