// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Calculator.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "UObject/NoExportTypes.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "SightCalculator.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable, EditInlineNew)
class PESTILENCE_API USightCalculator : public UObject, public ICalculator
{
	GENERATED_BODY()

	public:

		USightCalculator();
	
		UPROPERTY(BlueprintReadOnly, Category="Pestilence Perception")
			TObjectPtr<APawn> PlayerPawn;
		UPROPERTY(BlueprintReadOnly, Category="Pestilence Perception")
			TObjectPtr<APawn> OwnerPawn;
		UPROPERTY(BlueprintReadOnly, Category="Pestilence Perception")
			TObjectPtr<UPestilenceMovementComponent> MovementComponent;
	
		UPROPERTY(BlueprintReadOnly, Category="Pestilence Perception")
			TObjectPtr<APestilenceAIController> OwnerController;

		UPROPERTY(BlueprintReadOnly, Category="Pestilence Perception")
			TObjectPtr<UAISenseConfig_Sight> SightConfig;
		UPROPERTY(BlueprintReadOnly, Category="Pestilence Perception")
			bool IsOnCloseConeOfVision;
	
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float PvSprint;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float PvStandar;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float PvStealth;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float SIn;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float SOut;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float MIsMoving;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float MIsNotMoving;
		
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float CloseAngle;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float MidAngle;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float FarAngle;
	
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float CloseDistance;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float MidDistance;
		UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Pestilence Perception")
			float FarDistance;

		UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Pestilence Perception")
			bool bOnShadow;
		UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Pestilence Perception")
			float VelocityThreshold;

		float CalculateAttention_Implementation() override; // This is called if CalculateAttention is not implemented in blueprint
	
};
