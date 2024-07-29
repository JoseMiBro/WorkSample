#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_NotifyGuard.generated.h"

UCLASS()
class PESTILENCE_API UBTTask_NotifyGuard : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_NotifyGuard(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};