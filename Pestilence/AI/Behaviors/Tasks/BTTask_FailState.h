#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FailState.generated.h"

UCLASS()
class PESTILENCE_API UBTTask_FailState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FailState(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(VisibleAnywhere)
	bool bPlayerKilled = false;
};
