#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UpdateFirstTargetPoint.generated.h"

UCLASS()
class PESTILENCE_API UBTTask_UpdateFirstTargetPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_UpdateFirstTargetPoint(FObjectInitializer const& ObjectInitializer);
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere)
		int TargetPointIndex = 0;
};