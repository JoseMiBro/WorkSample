#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pestilence/AI/Enums/EPestilenceTypeCharacter.h"
#include "PestilenceAIInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPestilenceAIInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class PESTILENCE_API IPestilenceAIInterface
{
	GENERATED_IINTERFACE_BODY()
	
public:
	virtual EPestilenceTypeCharacter GetTypeCharacter () = 0;
};