// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlagWaitingOnTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UBTTask_FlagWaitingOnTargetPoint : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool IsWaitingOnTargetPoint;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
