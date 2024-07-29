#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindNearestGuard.generated.h"

UCLASS()
class PESTILENCE_API UBTTask_FindNearestGuard : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FindNearestGuard(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};