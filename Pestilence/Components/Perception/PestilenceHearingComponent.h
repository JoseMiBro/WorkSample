// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "PestilenceHearingComponent.generated.h"

class UAISenseConfig_Hearing;
class APestilenceAIController;
struct FAIStimulus;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBottleBreakingSound,SoundType,SoundType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UPestilenceHearingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPestilenceHearingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	//Hearing Config
	UPROPERTY()
	TObjectPtr<APestilenceAIController> OwnerController;

	UPROPERTY(EditAnywhere, Instanced, Category = "Pestilence AI Hearing")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	
	UPROPERTY(EditAnywhere, Category= "Pestilence Perception")
	float RotationSpeed = 30.f;
	
	UFUNCTION()
		bool LookAtStimuliSource(FVector StimuliLocation);

	UFUNCTION()
		bool CheckForHearingState(EPestilenceStates& CurrentState) const;

	UFUNCTION()
		void ConfigHearing() const;
	
	UFUNCTION()
		void ProcessHearingStimuli(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, Category= "Pestilence Perception")
		float LookAtTimer = 0.0f;
	UPROPERTY(EditAnywhere, Category= "Pestilence Perception")
		float LookAtTimerReset = 3.f;
	UPROPERTY(EditAnywhere, Category= "Pestilence Perception")
		float LoseInteresetTime = 1.f;
	UPROPERTY(BlueprintAssignable)
		FBottleBreakingSound OnSoundHeard;
	
	UPROPERTY()
		FAIStimulus LookAtStumuli;

	UPROPERTY()
		bool bLookAtStimuliIsValid = false;

	UPROPERTY()
		bool bLookAtSomething = false;

	UPROPERTY()
		FName LastSoundType;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static FName SoundTypeToString(SoundType Sound);
};
