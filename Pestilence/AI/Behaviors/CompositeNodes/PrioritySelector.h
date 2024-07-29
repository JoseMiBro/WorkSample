// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "PrioritySelector.generated.h"

/**
 * 
 */

class UPriorityDecorator;

struct FBTPriorityMemory : public FBTCompositeMemory
{
	//Child to execute
	uint8 ChildToExecute;
	int8 ChildBasePriority = 0;
};

class UBTDecorator_UtilityFunction;

UCLASS()
class PESTILENCE_API UPrioritySelector : public UBTCompositeNode
{
	GENERATED_BODY()

	public:
		UPrioritySelector(const FObjectInitializer& ObjectInitializer);
		virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
		virtual uint16 GetInstanceMemorySize() const override;
		virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
	
	protected:
		UPriorityDecorator* FindChildPriorityDecorator(int32 ChildIndex) const;
		void AssignPriorityFromTable(UPriorityDecorator* PriorityDecorator,FBehaviorTreeSearchData& SearchData) const;
	
		bool EvaluateUtilityScores(FBehaviorTreeSearchData& SearchData, int8& ToExecute) const;
		bool PartialEvaluateUtilityScores(FBehaviorTreeSearchData& SearchData, int8& ToExecute, int8& BaseWeight) const;
		virtual void NotifyNodeActivation(FBehaviorTreeSearchData& SearchData) const override;

	#if WITH_EDITOR
		virtual FName GetNodeIconName() const override;
	#endif
};
