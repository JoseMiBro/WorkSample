// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PestilenceGameMode.generated.h"

class ANPCManager;
class UPestilenceAbility;
class UPickableAbility;
class AProjectilePredictionMesh;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxAbilitiesChanged, int, _MaxAbilities);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputMethodChanged, bool, _bUsingController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIBeginChaseDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIEndChaseDelegate);

UCLASS(minimalapi)
class APestilenceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APestilenceGameMode();

  UPickableAbility* GetPickableAbility();
  TArray<UPestilenceAbility*> GetAbilities();
  TArray<AProjectilePredictionMesh*> GetProjectilePredictionMeshes();
  TArray<AProjectilePredictionMesh*> GetProjectilePredictionBounceMeshes();
  AProjectilePredictionMesh* GetProjectilePredictionGoalMesh();
  
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
  void SetMaxAbilities(int _MaxAbilities);
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
  int GetMaxAbilities() const;
  
  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
  FOnMaxAbilitiesChanged OnMaxAbilitiesChanged;

  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Input")
  FOnInputMethodChanged OnInputMethodChanged;

  UPROPERTY(BlueprintAssignable, Category = "AI Events")
  FOnAIBeginChaseDelegate OnAIBeginChase;
  
  UPROPERTY(BlueprintAssignable, Category = "AI Events")
  FOnAIEndChaseDelegate OnAIEndChase;

  UFUNCTION()
  void HandleAIBeginChase();
  UFUNCTION()
  void HandleAIEndChase();
  
  // Returns true if the player is currentrly being chased
  UFUNCTION(BlueprintCallable)
  bool GetChaseState() { return CurrentGuardsChasing > 0; };
  
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
  void SetKeyboardControl();
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
  void SetControllerControl();
  UPROPERTY(BlueprintReadOnly, Category = "Pestilence|Input")
  bool bUsingController = false;
  
  UPROPERTY(BlueprintReadWrite, Category = "Pestilence|Save")
  bool bIntegrityUnlocked = false;

  // Intrgrity threshold to set OnLoad
  UPROPERTY(EditAnywhere, Category = "Pestilence|CheckpointLoad")
  float IntegrityThreshold = 25.f;
  // Integrity on load if under threshold
  UPROPERTY(EditAnywhere, Category = "Pestilence|CheckpointLoad")
  float IntegritySetOnLoad = 35.f;

  ANPCManager* GetNPCManager();

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

private:
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TArray<TSubclassOf<UPestilenceAbility>> Abilities;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TSubclassOf<UPickableAbility> PickableAbilityClass;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TSubclassOf<AProjectilePredictionMesh> ProjectilePredictionMeshClass;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TSubclassOf<AProjectilePredictionMesh> ProjectilePredictionBounceMeshClass;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TSubclassOf<AProjectilePredictionMesh> ProjectilePredictionGoalMeshClass;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  int MaxProjectilePredictionMeshes = 100;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  int MaxProjectilePredictionBounceMeshes = 10;
  UPROPERTY()
  TArray<TObjectPtr<UPestilenceAbility>> AbilityObjects;
  UPROPERTY()
  TObjectPtr<UPickableAbility> PickableAbility;
  UPROPERTY()
  TArray<TObjectPtr<AProjectilePredictionMesh>> ProjectilePredictionMeshes;
  UPROPERTY()
  TArray<TObjectPtr<AProjectilePredictionMesh>> ProjectilePredictionBounceMeshes;
  UPROPERTY()
  TObjectPtr<AProjectilePredictionMesh> ProjectilePredictionGoalMesh;

  UPROPERTY()
  int CurrentGuardsChasing = 0;

  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  int MaxAbilities = 1;
  
  UPROPERTY(EditAnywhere, Category = "Pestilence|NPCManager")
    TSubclassOf<ANPCManager> NPCManagerClass;

  UPROPERTY(VisibleAnywhere, Category = "Pestilence|NPCManager")
    TObjectPtr<ANPCManager> NPCManager;
  
};



