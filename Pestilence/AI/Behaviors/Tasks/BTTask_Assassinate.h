#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Assassinate.generated.h"

UCLASS()
class PESTILENCE_API UBTTask_Assassinate : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Assassinate(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};