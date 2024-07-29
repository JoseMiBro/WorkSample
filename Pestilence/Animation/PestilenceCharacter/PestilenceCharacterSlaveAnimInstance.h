// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../Components/AI/AIReactionComponent.h"
#include "../../AI/PestilenceAIController.h"
#include "Pestilence/Enums/Gender.h"
#include "PestilenceCharacterSlaveAnimInstance.generated.h"

class APestilenceCharacter;
class UPestilenceMovementComponent;
class APestilenceAIController;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceCharacterSlaveAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
};
