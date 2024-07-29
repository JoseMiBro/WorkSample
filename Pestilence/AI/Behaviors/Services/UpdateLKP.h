#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "UpdateLKP.generated.h"

UCLASS()
class PESTILENCE_API UUpdateLKP : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UUpdateLKP();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere , Category = "Debug")
		bool bDrawDebug = false;

	UPROPERTY(EditAnywhere , Category = "Debug")
		float LifeTime = 5.f;
protected:
	virtual FString GetStaticServiceDescription() const override;
};
