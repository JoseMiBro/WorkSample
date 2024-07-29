#pragma once

#include "HidingSpotType.generated.h"

UENUM(BlueprintType)
enum class EHidingSpotType : uint8
{
  Fixed           UMETA(DisplayName = "Fixed"),
  CameraMovement  UMETA(DisplayName = "Camera Movement"),
  FreeMovement    UMETA(DisplayName = "Free Movement"),
};