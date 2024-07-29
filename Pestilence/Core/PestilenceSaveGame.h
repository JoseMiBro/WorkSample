#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PestilenceSaveGame.generated.h"

class APestilenceCharacter;
struct FPostProcessBlendParameter;

// PestChars save struct
USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	/* Identifier for which Actor this belongs to */
	UPROPERTY()
		FGuid CharacterID;

	/* For movable Actors, keep location,rotation,scale. */
	UPROPERTY()
		FTransform Transform;

	UPROPERTY()
		bool bIsDead = false;

	UPROPERTY()
		int SavedTargetPointIndex = 0;
};

// CheckPoint save struct
USTRUCT()
struct FCheckPointSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FGuid CheckPointID;

	UPROPERTY()
		bool bIsConsumed = false;
};

// Door save struct
USTRUCT()
struct FDoorSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FGuid DoorID;

	UPROPERTY()
		bool bIsLocked = false;

	UPROPERTY()
		bool bIsOpen = false;
};

//TargetPoint Save Struct
USTRUCT()
struct FTargetPointSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FGuid TargetPointID;

	UPROPERTY()
	bool bIsPointActive = false;
};

UCLASS()
class PESTILENCE_API UPestilenceSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPestilenceSaveGame();

	UPROPERTY(BlueprintReadWrite, Category = "Player Info")
		FTransform PlayerTransform;

	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<APestilenceCharacter> SavedPestChar = nullptr;

  UPROPERTY()
	TSubclassOf<APestilenceCharacter> SavedPestClass;

	UPROPERTY()
		FGuid SavedPestCharID;

	UPROPERTY()
		FVector SavedPestCharOGLocation;

	UPROPERTY()
		TArray<FActorSaveData> SavedActors;

	UPROPERTY()
		TArray<FCheckPointSaveData> SavedCheckPoints;

	UPROPERTY()
		TArray<FDoorSaveData> SavedSwingDoors;

	UPROPERTY()
		TArray<FTargetPointSaveData> SavedTargetPoints;

	UPROPERTY()
		TArray<FPostProcessBlendParameter> SavedBlendParameters;

	UPROPERTY()
		float SavedIntegrity;

	UPROPERTY()
		TArray<int> DiaryActiveEntrances;

  UPROPERTY()
    int MaxAbilities;

  UPROPERTY()
		bool bIsInCorrectZone;

	UPROPERTY(BlueprintReadOnly)
		FDateTime SaveDate;

	UPROPERTY(BlueprintReadOnly)
		float TotalPlayTime;

	UPROPERTY(BlueprintReadOnly)
		FString FormattedInfo;

	UPROPERTY(BlueprintReadWrite, Category = "Widget Info")
		int ObjectiveIndex;

  UPROPERTY(BlueprintReadWrite, Category = "Persistent Data")
    int CheckpointPriority = 0;

  UPROPERTY(BlueprintReadWrite, Category = "Persistent Data")
    bool bTutoDialogSeen = false;

  UPROPERTY(BlueprintReadWrite, Category = "Persistent Data")
    bool bHUBVastSeen = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Persistent Data")
		bool bTutoVastKilledData = false;

  UPROPERTY(BlueprintReadWrite, Category = "Persistent Data")
    bool bAlmacenVastKilled = false;

  UPROPERTY(BlueprintReadWrite, Category = "Persistent Data")
    bool bIntegrityUnlocked = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persistent Data")
    TMap<FName, bool> WorldState;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persistent Data")
    int TutorialsSeenIndex;
};
