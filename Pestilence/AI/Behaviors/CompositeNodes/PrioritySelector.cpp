 // Fill out your copyright notice in the Description page of Project Settings.


#include "PrioritySelector.h"

#include "BlueprintNodeHelpers.h"
#include "Pestilence/AI/Behaviors/Decorators/PriorityDecorator.h"

UPrioritySelector::UPrioritySelector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "PrioritySelector";
	bUseNodeActivationNotify = true;
	bUseChildExecutionNotify = true;
	
}

void UPrioritySelector::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	FBTPriorityMemory* UtilityMemory = reinterpret_cast<FBTPriorityMemory*>(NodeMemory);
	if (InitType == EBTMemoryInit::Initialize)
	{
		new (&UtilityMemory->ChildToExecute) uint8();
		new (&UtilityMemory->ChildBasePriority) int8();
	}
}

UPriorityDecorator* UPrioritySelector::FindChildPriorityDecorator(int32 ChildIndex) const
{
	auto const& ChildInfo = Children[ChildIndex];
	for (TObjectPtr<UBTDecorator> Dec : ChildInfo.Decorators)
	{
		const auto PriorityDecorator = Cast<UPriorityDecorator>(Dec);
		if(IsValid(PriorityDecorator))
			return PriorityDecorator;
	}
	//Child does not have a priority selector
	return nullptr;
}

void UPrioritySelector::AssignPriorityFromTable(UPriorityDecorator* PriorityDecorator,FBehaviorTreeSearchData& SearchData) const
{
	const EPestilenceStates State = PriorityDecorator->GetState();
	const TObjectPtr<APestilenceAIController> Controller = Cast<APestilenceAIController>(SearchData.OwnerComp.GetAIOwner());

	const TObjectPtr<FBTPriorityConfigRow> TableRow = Controller->BTPriorityConfig->FindRow<FBTPriorityConfigRow>(Controller->StateToString(State),"");
	if(TableRow)
	{
		PriorityDecorator->SetBasePriority(TableRow->StatePriority);
		PriorityDecorator->SetTimerReset(TableRow->MinStateTime);
	}
}

bool UPrioritySelector::EvaluateUtilityScores(FBehaviorTreeSearchData& SearchData, int8& ToExecute) const
{
	bool bIsNonZeroScore = false;
	float MaxPriority = 0;
	FBTPriorityMemory* NodeMemory = GetNodeMemory<FBTPriorityMemory>(SearchData);
	NodeMemory->ChildBasePriority = 0;
	
	//WARNING: If two childern have same priority, undefined behavior
	for(int32 Idx = 0; Idx < GetChildrenNum(); ++Idx)
	{
		//Check for (future) priority decorator
		auto PriorityDecorator = FindChildPriorityDecorator(Idx);
		if(IsValid(PriorityDecorator))
		{
			PriorityDecorator->ResetTimer(SearchData.OwnerComp.GetAIOwner()->GetBlackboardComponent());
			auto PriorityScore = PriorityDecorator->CalculatePriorityValue(
				SearchData.OwnerComp,
				PriorityDecorator->GetNodeMemory<uint8>(SearchData)
				);
			// Keep max priority child
			if(PriorityScore > MaxPriority)
			{
				MaxPriority = PriorityScore;
				ToExecute = Idx;
			}
			//Check for at least a child with SOME priority
			bIsNonZeroScore = bIsNonZeroScore || PriorityScore > 0.0f;
		}
	}
	return bIsNonZeroScore;
}

bool UPrioritySelector::PartialEvaluateUtilityScores(FBehaviorTreeSearchData& SearchData, int8& ToExecute, int8& BaseWeight) const
{
	bool bIsNonZeroScore = false;
	float MaxPriority = 0;

	//WARNING: If two childern have same priority, undefined behavior
	for(int32 Idx = 0; Idx < GetChildrenNum(); ++Idx)
	{
		//Check for (future) priority decorator
		auto PriorityDecorator = FindChildPriorityDecorator(Idx);
		if(IsValid(PriorityDecorator))
		{
			auto PriorityScore = PriorityDecorator->CalculatePriorityValue(
				SearchData.OwnerComp,
				PriorityDecorator->GetNodeMemory<uint8>(SearchData)
				);
			// Keep max priority child
			if(PriorityScore > MaxPriority && PriorityDecorator->GetBasePriority() >= BaseWeight)
			{
				MaxPriority = PriorityScore;
				ToExecute = Idx;
			}
			//Check for at least a child with SOME priority
			bIsNonZeroScore = bIsNonZeroScore || PriorityScore > 0.0f;
		}
	}
	if(bIsNonZeroScore)
		BaseWeight = FindChildPriorityDecorator(ToExecute)->GetBasePriority();
	return bIsNonZeroScore;
}

void UPrioritySelector::NotifyNodeActivation(FBehaviorTreeSearchData& SearchData) const
{
	// Evaluate utility scores for each child
	int8 ChildToExecute = -1;
	FBTPriorityMemory* NodeMemory = GetNodeMemory<FBTPriorityMemory>(SearchData);
	const bool bNonZeroUtility = EvaluateUtilityScores(SearchData, ChildToExecute);
	EPestilenceStates State =
		TEnumAsByte<EPestilenceStates>(
			SearchData.OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(Keys::State)
			);

	for(int32 i = 0; i < Children.Num(); ++i)
	{
		const TObjectPtr<UPriorityDecorator> Decorator = FindChildPriorityDecorator(i);
		if(IsValid(Decorator))
		{
			AssignPriorityFromTable(Decorator,SearchData);
			if(Decorator->GetState() != State)
				Decorator->ResetTimer(SearchData.OwnerComp.GetAIOwner()->GetBlackboardComponent());
		}
	}
	// Executes Child, only re-chooses child when ResetTree
	if(bNonZeroUtility)
		if(ChildToExecute != -1)
		{
			NodeMemory->ChildToExecute = ChildToExecute;
			NodeMemory->ChildBasePriority = FindChildPriorityDecorator(ChildToExecute)->GetBasePriority();
		}

}

int32 UPrioritySelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	FBTPriorityMemory* NodeMemory = GetNodeMemory<FBTPriorityMemory>(SearchData);

	check(NodeMemory->ChildToExecute <= GetChildrenNum());
	
	int8 NextChildIdx = NodeMemory->ChildToExecute;

	if(LastResult == EBTNodeResult::Aborted)
	{
		PartialEvaluateUtilityScores(SearchData,NextChildIdx,NodeMemory->ChildBasePriority);
		NodeMemory->ChildBasePriority = FindChildPriorityDecorator(NextChildIdx)->GetBasePriority();
	}
	
	return NextChildIdx;
}

uint16 UPrioritySelector::GetInstanceMemorySize() const
{
	return sizeof(FBTPriorityMemory);
}

#if WITH_EDITOR

FName UPrioritySelector::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Utility.Icon");
}

#endif
