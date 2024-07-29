// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FunctionWrapper.generated.h"

class UPriorityDecorator;
/**
 * 
 */
UCLASS(EditInlineNew)
class PESTILENCE_API UFunctionWrapper : public UObject
{
	GENERATED_BODY()

	public:

	UFUNCTION()
		virtual float CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp);
};
