#pragma once
#include "Archetype.generated.h"

UENUM(BlueprintType)
enum class EArchetype : uint8
{
	Dummy		UMETA(DisplayName = "Dummy"),
	Cleric		UMETA(DisplayName = "Cleric"),
	Noble		UMETA(DisplayName = "Noble"),
	Farmer		UMETA(DisplayName = "Farmer"),
	Worker		UMETA(DisplayName = "Worker")
};