#pragma once
#include "InteractableType.generated.h"

UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	  Default     UMETA(DisplayName = "Default")
	, Pickable    UMETA(DisplayName = "Pickable")
	, Climb       UMETA(DisplayName = "Climb")
	, Hiding      UMETA(DisplayName = "Hiding")
	, Door        UMETA(DisplayName = "Door")
	, OpenDoor    UMETA(DisplayName = "OpenDoor")
	, LockedDoor  UMETA(DisplayName = "LockedDoor")
	, BlockedDoor UMETA(DisplayName = "BlockedDoor")
	, NPC         UMETA(DisplayName = "NPC")
	, Judge       UMETA(DisplayName = "Judge")
	, Guard       UMETA(DisplayName = "Guard")
	, Foreman     UMETA(DisplayName = "Foreman")
	, Noble       UMETA(DisplayName = "Noble")
	, Worker      UMETA(DisplayName = "Worker")
	, Farmer      UMETA(DisplayName = "Farmer")
};