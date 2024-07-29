// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "CheckStateDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UCheckStateDecorator : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		EPestilenceStates StateToCheck;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const  override;
};
