// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PestilencePauseComponent.generated.h"

class APestilenceHubManager;
class APestilenceHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PESTILENCE_API UPestilencePauseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	APestilenceHubManager* pPestilenceHudManager;

	UPROPERTY(BlueprintReadWrite)
	APestilenceHUD* PestilenceHUD;

public:	
	// Sets default values for this component's properties
	UPestilencePauseComponent();
	bool bIsPauseOpen = false;
	bool bIsDiaryOpen = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void AddPauseMenu();

	UFUNCTION()
		void AddDiaryMenu();

	UFUNCTION(BlueprintCallable)
		void AddGameOverMenu();

	bool IsGamePaused();
};
