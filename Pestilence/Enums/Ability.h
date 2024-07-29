#pragma once
#include "Ability.generated.h"

UENUM(BlueprintType)
enum class EAbility : uint8
{
	  Dummy					UMETA(DisplayName = "Dummy")
	, Infection			UMETA(DisplayName = "Infection")
	, Teleport			UMETA(DisplayName = "Teleport")
	, Possession		UMETA(DisplayName = "Possession")
	, Pulse					UMETA(DisplayName = "Pulse")
	, Pickable			UMETA(DisplayName = "Pickable")
};