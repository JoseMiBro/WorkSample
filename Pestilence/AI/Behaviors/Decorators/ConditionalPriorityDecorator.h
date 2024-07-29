// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pestilence/AI/Behaviors/Decorators/PriorityDecorator.h"
#include "ConditionalPriorityDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UConditionalPriorityDecorator : public UPriorityDecorator
{
	GENERATED_BODY()

public:
	UConditionalPriorityDecorator(const FObjectInitializer& ObjectInitializer);
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BlackboardKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bKeyCondition;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	bool bCompareKeyValueTo; 
};
