#pragma once

#include "CoreMinimal.h"
#include "FunctionWrapper.h"
#include "ChaseWeightFunction.generated.h"

UCLASS()
class PESTILENCE_API UChaseWeightFunction : public UFunctionWrapper
{
	GENERATED_BODY()
public:
	virtual float CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp) override;
};
