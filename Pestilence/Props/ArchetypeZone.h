#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pestilence/Enums/Archetype.h"
#include "Pestilence/Enums/Gender.h"
#include "ArchetypeZone.generated.h"

class ANPCManager;

UCLASS()
class PESTILENCE_API AArchetypeZone : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "ArchetypeZone")
	TArray<EArchetype> RestrictedArchetypes;
	UPROPERTY(EditAnywhere, Category = "ArchetypeZone")
	TArray<EGender> RestrictedGenders;
	UPROPERTY(EditAnywhere, Category = "ArchetypeZone")
	bool bForbiddenZone;

	
	AArchetypeZone();

	UFUNCTION()
	void Refresh();

	UFUNCTION()
	void OnRefreshRequested();
	
	virtual void Tick(float DeltaTime) override;
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnActorEnter(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnActorExit(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	bool ActorIsRestricted(AActor* ActorToCheck) const;

private:
  TObjectPtr<ANPCManager> NPCManager;
	TObjectPtr<UShapeComponent> ShapeComponent;
	bool FirstFrame = true;
	TObjectPtr<class APestilenceCharacter> PrevPlayer;
};