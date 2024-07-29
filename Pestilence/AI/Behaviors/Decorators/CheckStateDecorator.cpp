// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckStateDecorator.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"

bool UCheckStateDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if( StateToCheck ==
		TEnumAsByte<EPestilenceStates>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(Keys::State)))
		return true;

	return false;
}
