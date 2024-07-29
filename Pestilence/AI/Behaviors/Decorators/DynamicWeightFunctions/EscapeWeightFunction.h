#pragma once

#include "CoreMinimal.h"
#include "Pestilence/AI/Behaviors/Decorators/DynamicWeightFunctions/FunctionWrapper.h"
#include "EscapeWeightFunction.generated.h"

UCLASS()
class PESTILENCE_API UEscapeWeightFunction : public UFunctionWrapper
{
	GENERATED_BODY()
public:
	virtual float CalculateDynamicWeight(const UPriorityDecorator* Decorator, UBehaviorTreeComponent* OwnerComp) override;
};
