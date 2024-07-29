#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UpdateLastKnownPoint.generated.h"

UCLASS()
class PESTILENCE_API UBTTask_UpdateLastKnownPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_UpdateLastKnownPoint(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};