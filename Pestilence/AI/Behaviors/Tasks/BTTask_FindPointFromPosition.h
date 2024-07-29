#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPointFromPosition.generated.h"

UCLASS()
class PESTILENCE_API UBTTask_FindPointFromPosition : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_FindPointFromPosition(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BlackboardResultKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Radius = 200.f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float DistancePoint = 250.f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FVector3d PlayerDirection;
};
