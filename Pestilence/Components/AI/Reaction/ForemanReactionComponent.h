#pragma once

#include "CoreMinimal.h"
#include "Pestilence/Components/AI/AIReactionComponent.h"
#include "ForemanReactionComponent.generated.h"

UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class PESTILENCE_API UForemanReactionComponent : public UAIReactionComponent
{
	GENERATED_BODY()

public:
	UForemanReactionComponent();

	virtual void React_Implementation(APestilenceAIController* PestilenceAIController, EPestilenceTypeCharacter TypeCharacter) override;
};