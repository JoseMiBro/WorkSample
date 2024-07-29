#pragma once

UENUM(BlueprintType)
enum class ESuspiciousActionType : uint8
{
	Default         UMETA(DisplayName = "Default"),
	Climb           UMETA(DisplayName = "Climb"),
	Degradation	    UMETA(DisplayName = "Degradation"),
	RestrictedArea  UMETA(DisplayName = "RestrictedArea"),
	DefaultAbility  UMETA(DisplayName = "DefaultAbility"),
	PrepareAbility  UMETA(DisplayName = "PrepareAbility"),
	TakeDown        UMETA(DisplayName = "TakeDown"),
	Crouch          UMETA(DisplayName = "Crouch"),
	PickDoor        UMETA(DisplayName = "PickDoor"),
};