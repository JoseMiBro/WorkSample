#pragma once

#include "CoreMinimal.h"
#include "NavAreas/NavArea.h"
#include "NavArea_Interact.generated.h"

UCLASS()
class PESTILENCE_API UNavArea_Interact : public UNavArea
{
	GENERATED_BODY()
public:
	UNavArea_Interact(const FObjectInitializer& ObjectInitializer);
};
