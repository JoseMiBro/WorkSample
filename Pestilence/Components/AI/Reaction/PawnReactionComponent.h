#pragma once

#include "CoreMinimal.h"
#include "Pestilence/Components/AI/AIReactionComponent.h"
#include "PawnReactionComponent.generated.h"

UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class PESTILENCE_API UPawnReactionComponent : public UAIReactionComponent
{
	GENERATED_BODY()

public:
	UPawnReactionComponent();
	
	virtual void React_Implementation(APestilenceAIController* PestilenceAIController, EPestilenceTypeCharacter TypeCharacter) override;
};