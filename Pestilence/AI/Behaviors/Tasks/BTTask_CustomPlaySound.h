// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CustomPlaySound.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESoundToPlay : uint8
{
	InfectionSound,
	LostInterest
};

UCLASS()
class PESTILENCE_API UBTTask_CustomPlaySound : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		ESoundToPlay Sound;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
