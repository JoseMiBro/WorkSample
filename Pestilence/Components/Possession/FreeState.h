#pragma once
#include "CoreMinimal.h"
#include "PossessionState.h"
#include "FreeState.generated.h"

UCLASS()
class PESTILENCE_API UFreeState : public UPossessionState
{
	GENERATED_BODY()

	virtual void UpdateState(float DeltaTime) override;
};