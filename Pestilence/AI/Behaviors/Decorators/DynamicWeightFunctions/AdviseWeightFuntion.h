// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pestilence/AI/Behaviors/Decorators/DynamicWeightFunctions/FunctionWrapper.h"
#include "AdviseWeightFuntion.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UAdviseWeightFuntion : public UFunctionWrapper
{
	GENERATED_BODY()
public:
	virtual float CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp) override;
};
