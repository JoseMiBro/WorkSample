// Fill out your copyright notice in the Description page of Project Settings.


#include "DoormanWeightFunction.h"

float UDoormanWeightFunction::CalculateDynamicWeight(const UPriorityDecorator* Decorator,
	UBehaviorTreeComponent* OwnerComp)
{
	return Super::CalculateDynamicWeight(Decorator, OwnerComp);
}
