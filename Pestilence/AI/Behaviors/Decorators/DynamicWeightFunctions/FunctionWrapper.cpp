// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionWrapper.h"
#include "../PriorityDecorator.h"

float UFunctionWrapper::CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp)
{
	return Decorator->GetBasePriority();
}