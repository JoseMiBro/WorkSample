#include "Pestilence/AI/Behaviors/Tasks/BTTask_ChangeBool.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"

UBTTask_ChangeBool::UBTTask_ChangeBool(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Change Bool";
	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ChangeBool, BlackboardKey));
}

EBTNodeResult::Type UBTTask_ChangeBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const FName BlackboardKeyName = BlackboardKey.SelectedKeyName;

	const FString StringBBKey = BlackboardKeyName.ToString();

	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	if(PestilenceAIController == nullptr)
		return EBTNodeResult::Failed;

	const bool bKeyValue = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BlackboardKeyName);
	
	if (bKeyValue == bValue)
	{
		return EBTNodeResult::Succeeded;
	}

	PestilenceAIController->GetBlackboard()->SetValueAsBool(BlackboardKeyName, bValue);

	return EBTNodeResult::Succeeded;
}

FString UBTTask_ChangeBool::GetStaticDescription() const
{
	return FString("Change Bool Value");
}