#pragma once

#include "CoreMinimal.h"
#include "NavLinkCustomComponent.h"
#include "PestilenceNavLinkComponent.generated.h"

UCLASS()
class PESTILENCE_API UPestilenceNavLinkComponent : public UNavLinkCustomComponent
{
	GENERATED_BODY()

public:
	UPestilenceNavLinkComponent(const FObjectInitializer& ObjectInitializer);
};
