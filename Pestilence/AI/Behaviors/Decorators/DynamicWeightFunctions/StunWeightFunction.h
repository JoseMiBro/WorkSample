// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pestilence/AI/Behaviors/Decorators/DynamicWeightFunctions/FunctionWrapper.h"
#include "StunWeightFunction.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UStunWeightFunction : public UFunctionWrapper
{
	GENERATED_BODY()
	
	virtual float CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp) override;
};
