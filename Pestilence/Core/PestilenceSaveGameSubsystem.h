#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Pestilence/Core/PestilenceSaveGame.h"
#include "PestilenceSaveGameSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndLoadDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndSaveDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSuccesfulSave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFailedSave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoSaveFound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTutoDialogSeen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHUBVastSeen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTutoVastKilledData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAlmacenVastKilled);



class UMaterialParameterCollection;
class ULevelStreaming;
struct FTimerHandle;

UCLASS()
class PESTILENCE_API UPestilenceSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		FString CurrentSlotName = "SaveSlot_0"; // Default QuickSlot

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// End USubsystem
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	FString GetCurrentSlotName() { return CurrentSlotName; }

	UFUNCTION(BlueprintCallable)
		void SetSlotName(FString& NewSlotName);

	//Default SaveSlot_0 for QuickSave
	UFUNCTION(BlueprintCallable)
		void ResetSlotName() { CurrentSlotName = "SaveSlot_0"; }

	UFUNCTION(BlueprintCallable)
		void SetCheckPointSlotName() { CurrentSlotName = "CheckPoint_Slot"; }
	UFUNCTION(BlueprintCallable)
		void SetSlot1Name() { CurrentSlotName = "SaveSlot_1"; }
	UFUNCTION(BlueprintCallable)
		void SetSlot2Name() { CurrentSlotName = "SaveSlot_2"; }
	UFUNCTION(BlueprintCallable)
		void SetSlot3Name() { CurrentSlotName = "SaveSlot_3"; }

	UFUNCTION(BlueprintCallable)
		void QuickSave();
	UFUNCTION(BlueprintCallable)
		void QuickLoad();

	UFUNCTION(BlueprintCallable)
		void ReLoad();

	// Returns true if there is no checkpoints saved or there is not current saveslot
	UFUNCTION(BlueprintCallable)
		bool IsCheckPointArrayEmpty();

	UFUNCTION()
	void OnSavedCharacterPossessed();

	void ResetParameters();

	UPROPERTY(BlueprintAssignable)
		FLoadDelegate OnLoad;

	UPROPERTY(BlueprintAssignable)
		FSaveDelegate OnSave;

	UPROPERTY(BlueprintAssignable)
		FEndLoadDelegate OnEndLoad;

  UPROPERTY(BlueprintAssignable)
    FEndSaveDelegate OnEndSave;

  UPROPERTY(BlueprintAssignable)
    FSuccesfulSave OnSuccesfulSave;

  UPROPERTY(BlueprintAssignable)
    FFailedSave OnFailedSave;

  UPROPERTY(BlueprintAssignable)
    FFailedSave OnNoSaveFound;

  UPROPERTY(BlueprintAssignable)
		FTutoDialogSeen TutoDialogSeen;

  UPROPERTY(BlueprintAssignable)
		FHUBVastSeen HUBVastSeen;

  UPROPERTY(BlueprintAssignable)
		FTutoVastKilledData TutoVastKilledData;

  UPROPERTY(BlueprintAssignable)
		FAlmacenVastKilled AlmacenVastKilled;

  bool bShouldLoad = false;
  FString LoadedSlot = "SaveSlot_0"; // Setear y usar esto en resetslotname, solo setear cuando se usa un load desde menu?

  UPROPERTY()
  TArray<FDoorSaveData> SavedSwingDoors;


private:

	void LogIfGameWasSavedOrNot(const bool IsSaved);

	void CheckStreamingVolume(const FVector& _Location);

	FString FormatDateInfo(FDateTime& _SavedTime, float& _PlayTime);

	UFUNCTION()
	void OnLevelLoaded();

	int LevelsToLoad;
	int LevelsLoaded = 0;
	bool bShouldRemoveLoadingScreen = false;

	float LoadTimeDilation = 0.001f;
  float LoadFailsafeTime = 10.f;
  FTimerHandle TimerHandle;

	UPROPERTY()
		TObjectPtr<UPestilenceSaveGame> SaveGameObject;

  UPROPERTY()
	TArray<ULevelStreaming*> StreamedLevels;
};
