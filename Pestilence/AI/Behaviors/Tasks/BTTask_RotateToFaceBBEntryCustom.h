// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"
#include "BTTask_RotateToFaceBBEntryCustom.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UBTTask_RotateToFaceBBEntryCustom : public UBTTask_RotateToFaceBBEntry
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
