#pragma once

#include "CoreMinimal.h"
#include "Pestilence/Components/AI/AIReactionComponent.h"
#include "GuardReactionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UGuardReactionComponent : public UAIReactionComponent
{
	GENERATED_BODY()

public:
	UGuardReactionComponent();
	
	virtual void React_Implementation(APestilenceAIController* PestilenceAIController, EPestilenceTypeCharacter TypeCharacter) override;
};
