#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPointFarFromPlayer.generated.h"

UCLASS()
class PESTILENCE_API UBTTask_FindPointFarFromPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FindPointFarFromPlayer(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, Category = "Variables")
		float Radius = 1500.f;
	
	UPROPERTY(EditAnywhere, Category = "Variables")
		float Distance = 2000.f;
};