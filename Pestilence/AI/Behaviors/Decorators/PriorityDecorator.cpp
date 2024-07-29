// Fill out your copyright notice in the Description page of Project Settings.


#include "PriorityDecorator.h"

#include "../CompositeNodes/PrioritySelector.h"


UPriorityDecorator::UPriorityDecorator(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	// Settear Timer segun blackboard key
	bNotifyTick = true;
	bNotifyActivation = true;
	bAllowAbortChildNodes = true;
	bNotifyBecomeRelevant = true;
}

float UPriorityDecorator::CalculatePriorityValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return DynamicWeightFunction->CalculateDynamicWeight(this, &OwnerComp);
}
void UPriorityDecorator::ResetTimer(UBlackboardComponent* BlackboardComponent)
{
	BlackboardComponent->SetValueAsFloat(
		Keys::Timer,
		0.f
		);
}

void UPriorityDecorator::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<UBlackboardComponent> BlackboardComponent = OwnerComp.GetBlackboardComponent();
	
	float Timer = BlackboardComponent->GetValueAsFloat(Keys::Timer) + DeltaSeconds;
	
	BlackboardComponent->SetValueAsFloat(
	Keys::Timer,
	Timer
	);

	int Result = 0;
	
	for(int i = 0; i < CheckForAbortFunctions.Num(); ++i )
	{
		Result += CheckForAbortFunctions[i]->CalculateDynamicWeight(this,&OwnerComp);
		if(Result > 0)
			break;
	}
	
	if(Result > 0 || Timer >= TimerReset)
		OwnerComp.RestartTree();
}

float UPriorityDecorator::GetBasePriority() const
{
	return BasePriority;
}

void UPriorityDecorator::SetBasePriority(float NewBasePriority)
{
	BasePriority = NewBasePriority;
}

EPestilenceStates UPriorityDecorator::GetState() const
{
	return State;
}

void UPriorityDecorator::SetTimerReset(float NewTimerReset)
{
	TimerReset = NewTimerReset;
}
