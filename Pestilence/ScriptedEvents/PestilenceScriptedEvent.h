// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PestilenceScriptedEvent.generated.h"

class ULevelStreaming;
class APestilenceTargetPoint;
class FName;

/**
 * 
 */
UCLASS()
class PESTILENCE_API APestilenceScriptedEvent : public AActor
{
	GENERATED_BODY()

public:
	/*
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Scripted Event")
	void LoadSublevels();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Scripted Event")
	void UnloadSublevels();
	*/
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Scripted Event")
	void ActivateTargetPoints();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Scripted Event")
	void DeactivateTargetPoints();

private:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Scripted Event", meta = (AllowPrivateAccess = "true"))
  TArray<FName> SublevelsToLoad;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Scripted Event", meta = (AllowPrivateAccess = "true"))
  TArray<FName> SublevelsToUnload;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Scripted Event", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<APestilenceTargetPoint>> TargetPointsToActivate;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Scripted Event", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<APestilenceTargetPoint>> TargetPointsToDeactivate;
};
