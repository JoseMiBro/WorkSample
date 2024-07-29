#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "ChangeSpeed.generated.h"

UCLASS()
class PESTILENCE_API UChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UChangeSpeed();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual FString GetStaticServiceDescription() const override;
private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed = 150.f;
};