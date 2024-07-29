// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "PestilenceInputModifier.generated.h"

/**
 * 
 */
UCLASS()

class PESTILENCE_API UPestilenceInputModifier : public UInputModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float AccelerationDeadzone = 0.5f;

	UPROPERTY(EditAnywhere)
	float SecondsToMaxSpeed = 2.0f;

protected:
	virtual	FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	float AccelerationX;
	float AccelerationY;
};

UCLASS()
class PESTILENCE_API UPestilenceInputControllerModifier : public UInputModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float AccelerationDeadzone = 0.5f;

	UPROPERTY(EditAnywhere)
	float SecondsToMaxSpeed = 2.0f;

protected:
	virtual	FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	float AccelerationX;
	float AccelerationY;
};
