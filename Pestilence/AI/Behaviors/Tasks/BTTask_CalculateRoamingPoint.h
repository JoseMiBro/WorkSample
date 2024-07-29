#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CalculateRoamingPoint.generated.h"

UCLASS()
class PESTILENCE_API UBTTask_CalculateRoamingPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_CalculateRoamingPoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	//Variables
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BlackboardResultKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Radius = 250.f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float DistancePoint = 250.f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float RaycastLength = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float OffsetDistance = 65.f;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FVector Direction = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bDrawDebug = false;
};
