// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionWrapper.h"
#include "AlertWeightFunction.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UAlertWeightFunction : public UFunctionWrapper
{
	GENERATED_BODY()

	public:
	virtual float CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp) override;
};
