// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/AI/Behaviors/Decorators/ConditionalPriorityDecorator.h"

UConditionalPriorityDecorator::UConditionalPriorityDecorator(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UConditionalPriorityDecorator, BlackboardKey));
}

void UConditionalPriorityDecorator::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bKeyCondition = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BlackboardKey.SelectedKeyName);
	TObjectPtr<UBlackboardComponent> BlackboardComponent = OwnerComp.GetBlackboardComponent();
	float Timer = 0.f;
	
	if (bKeyCondition == bCompareKeyValueTo)
	{
		//Timer += DeltaSeconds;
		Timer = BlackboardComponent->GetValueAsFloat(Keys::Timer) + DeltaSeconds;
		BlackboardComponent->SetValueAsFloat(
	Keys::Timer,
			Timer
		);
	}
	else
	{
		BlackboardComponent->SetValueAsFloat(
	Keys::Timer,
	0.f
		);
		Timer = 0.f;
	}

	int Result = 0;

	for(int i = 0; i < CheckForAbortFunctions.Num(); ++i )
	{
		Result += CheckForAbortFunctions[i]->CalculateDynamicWeight(this,&OwnerComp);
		if(Result > 0)
			break;
	}

	
	if(Result > 0)
		OwnerComp.RestartTree();

	if(Timer >= TimerReset)
	{
		const FName BlackboardKeyNameBool = BlackboardKey.SelectedKeyName;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKeyNameBool, !bCompareKeyValueTo);
		OwnerComp.RestartTree();
	}
}