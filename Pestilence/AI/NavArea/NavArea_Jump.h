#pragma once

#include "CoreMinimal.h"
#include "NavAreas/NavArea.h"
#include "NavArea_Jump.generated.h"

UCLASS()
class PESTILENCE_API UNavArea_Jump : public UNavArea
{
	GENERATED_BODY()
public:
	UNavArea_Jump(const FObjectInitializer& ObjectInitializer);
};
