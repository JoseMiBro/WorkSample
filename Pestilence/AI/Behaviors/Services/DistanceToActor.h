#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "DistanceToActor.generated.h"

UCLASS()
class PESTILENCE_API UDistanceToActor : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UDistanceToActor();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	FString GetStaticServiceDescription() const override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BlackboardActorKey;
};