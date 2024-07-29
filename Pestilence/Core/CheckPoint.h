#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

UCLASS()
class PESTILENCE_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACheckPoint(const FObjectInitializer& ObjectInitializer);
	virtual void OnConstruction
	(
		const FTransform& Transform
	) override;

	bool GetCheckPointState() { return bConsumed; }
	void SetCheckPointState(bool bState) { bConsumed = bState; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Savegame")
	FGuid ID;
	
	// Restore Inegrity OnLoad, Values given in GameMode
	UPROPERTY(EditAnywhere, Category = "Savegame")
	bool bRestoreIntegrity = true;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UArrowComponent* SpawnLocation;

	// Saves once and consumes the checkpoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bConsumed = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Savegame")
	int CheckpointPriority = 0;
	UPROPERTY()
	FString CheckPointName = "CheckPoint_Slot";

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
