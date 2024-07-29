#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "SetPestilenceState.generated.h"

UCLASS()
class PESTILENCE_API USetPestilenceState : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	USetPestilenceState();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, Category = "State")
	EPestilenceStates NewState = EPestilenceStates::None;
};
