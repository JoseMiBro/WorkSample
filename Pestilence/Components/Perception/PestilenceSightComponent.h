// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pestilence/AI/Enums/EPestilenceTypeCharacter.h"
#include "Pestilence/Calculators/SightCalculator.h"
#include "Pestilence/Enums/SuspiciousActionType.h"
#include "SuspiciousAction.h"
#include "PestilenceSightComponent.generated.h"

class UAIReactionComponent;
class APestilenceAIController;
class UAISenseConfig_Sight;
class UPestilenceCharacterAnimInstance;
struct FAIStimulus;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShadowChanged, bool, _bOnShadow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSuspiciousActionReceived, const FSuspiciousAction&, SuspiciousAction);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UPestilenceSightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPestilenceSightComponent();

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Pestilence Perception")
		bool bOnSight = false;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Pestilence Perception")
		bool bOnShadow = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pestilence Perception")
		bool bSightAI = false;
	
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Archetype")
		FOnShadowChanged OnShadowChanged;

	bool bPlayerSighted = false;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence Perception")
		FOnSight OnPlayerSighted;

	UPROPERTY()
		bool IsOnCloseConeOfVision;
	
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Archetype")
		FOnSuspiciousActionReceived OnSuspiciousActionReceived;

	UFUNCTION()
		void ChangeVisionConfig(EPestilenceStates State);	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
		virtual void ProcessSightStimuli(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
		virtual void ConfigSight();
	
	UPROPERTY()
		TObjectPtr<APestilenceAIController> OwnerController;
private:	
	//Sight Config

	UPROPERTY()
		TObjectPtr<APestilenceCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, Instanced, Category = "Pestilence AI Sight")
		TObjectPtr<UAISenseConfig_Sight> SightConfig;
	UPROPERTY(EditAnywhere, Instanced, Category = "Pestilence AI Sight")
		TObjectPtr<UAISenseConfig_Sight> WideSightConfig;
	
	UPROPERTY(EditAnywhere, Instanced, Category = "Pestilence AI Sight")
		TObjectPtr<USightCalculator> SightCalculator;

	UPROPERTY(EditAnywhere, Category = "Pestilence Type Character")
		EPestilenceTypeCharacter OwnerType = EPestilenceTypeCharacter::Pawn;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence Reaction")
		TObjectPtr<UAIReactionComponent> ReactionComponent;
	UPROPERTY()
		TObjectPtr<UPestilenceCharacterAnimInstance> CharacterAnimationInstance;

public:
	UPROPERTY(VisibleAnywhere, Category="Pestilence Reaction")
		TArray<TObjectPtr<AActor>> ActorsOnSight;
private:
	
	UPROPERTY(EditAnywhere, Category="Pestilence Perception")
		float PatrolAttentionLose = 5.f;
	UPROPERTY(EditAnywhere, Category="Pestilence Perception")
		float StandarAttentionLose = 5.f;
	UPROPERTY(EditAnywhere, Category="Pestilence Perception")
		float AlertAttentionLose = 3.f;
	UPROPERTY(EditAnywhere, Category="Pestilence Perception")
		float ChaseAttentionLose = 1.f;
	
	UPROPERTY(EditAnywhere,Category="Pestilence Perception")
		float RotationSpeed = 20.f;

	bool IsOnConeOfVision;
	UPROPERTY(EditAnywhere,Category="Pestilence Perception")
		bool bDrawDebugCloseConeOfVision = false;

	
	UFUNCTION()
		void CalculateVisibilityFactor() const;
	UFUNCTION()
		void CalculateAttentionLost(float DeltaTime);
	UFUNCTION()
		void SuspiciousActionReceived(const FSuspiciousAction& SuspiciousAction);

	void ControlOnSight(bool IsOnSight,AActor* Actor);
	UPROPERTY(EditAnywhere,Category="Pestilence Perception")
		float CloseSightDistance;
	UPROPERTY(EditAnywhere,Category="Pestilence Perception")
		float CloseSightHalfAngle = 30.f;
	UPROPERTY(EditAnywhere,Category="Pestilence Perception")
		float BackwardConeOffset = 20.f;
	UPROPERTY(EditAnywhere,Category="Pestilence Perception")
		float CloseSightConeHeightOffset = 48.f;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void SetOnShadow(bool IsOnShadow);
	UFUNCTION()
		void Init();
	UFUNCTION()
		void ControlActorsOnSight(bool Add, AActor* Actor);
	UFUNCTION()
		void CheckOnSight();
	UFUNCTION()
		void ControlSuspiciosActionSubscription(const AActor* Actor);
	UFUNCTION()
		void ControlSightOnPossesion();
	UFUNCTION()
		void RotateToLookAtPlayer() const;
};
