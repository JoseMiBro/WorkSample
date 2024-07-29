#pragma once
#include "Gender.generated.h"

UENUM(BlueprintType)
enum class EGender : uint8
{
	Other		UMETA(DisplayName = "Other"),
	Male		UMETA(DisplayName = "Male"),
	Female		UMETA(DisplayName = "Female")
};