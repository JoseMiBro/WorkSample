#pragma once

#include "CoreMinimal.h"
#include "Pestilence/AI/Behaviors/Decorators/DynamicWeightFunctions/FunctionWrapper.h"
#include "WatchdogWeightFunction.generated.h"

UCLASS()
class PESTILENCE_API UWatchdogWeightFunction : public UFunctionWrapper
{
	GENERATED_BODY()
public:
	virtual float CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp) override;
};
