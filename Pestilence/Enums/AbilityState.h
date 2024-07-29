#pragma once
#include "AbilityState.generated.h"

UENUM(BlueprintType)
enum class EAbilityState : uint8
{
	  Idle								UMETA(DisplayName = "Idle")
	, Prepare							UMETA(DisplayName = "Prepare")
	, PrepareNoIntegrity  UMETA(DisplayName = "PrepareNoIntegrity")
	, Cast								UMETA(DisplayName = "Cast")
	, Use									UMETA(DisplayName = "Use")
};