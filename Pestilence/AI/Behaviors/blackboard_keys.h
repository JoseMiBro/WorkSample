#pragma once

#include "Engine/DataTable.h"
#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

namespace Keys
{
	/** Keys that are configured on the BlackBoard */

	//***********************GENERIC KEYS***********************

    //Enum
	TCHAR const* const State = TEXT("State");
	TCHAR const* const LastState = TEXT("LastState");
	
	//Object
	TCHAR const* const Self = TEXT("SelfActor");
	TCHAR const* const Player = TEXT("Player");

	//Vector
	TCHAR const* const TargetPointPosition = TEXT("TargetPointPosition");
	TCHAR const* const LastKnownPoint = TEXT("LastKnownPoint");
	TCHAR const* const PestilenceCharacterLKP = TEXT("PestilenceCharacterLKP");
	TCHAR const* const Rotate = TEXT("Rotate");
	TCHAR const* const RandomPoint = TEXT("RandomPoint");
	TCHAR const* const PlayerPosition = TEXT("PlayerPosition");
	TCHAR const* const PlayerOrientation = TEXT("PlayerOrientation");
	TCHAR const* const PlayerMovementDirection = TEXT("PlayerMovementDirection");

	//Float
	TCHAR const* const WaitTime = TEXT("WaitTime");
	TCHAR const* const StunTime = TEXT("StunTime");

	TCHAR const* const Timer = TEXT("Timer");
	TCHAR const* const Alert = TEXT("Alert");

	//Int
	TCHAR const* const TargetPointIndex = TEXT("TargetPointIndex");

	//Bool
	TCHAR const* const Static = TEXT("Static");
	TCHAR const* const Witness = TEXT("Witness");
	TCHAR const* const InitialPosition = TEXT("InitialPosition");
	TCHAR const* const SeeingPlayer = TEXT("SeeingPlayer"); 
	TCHAR const* const SomeOneInfected = TEXT("SomeOneInfected"); 
	TCHAR const* const CanGetAttention = TEXT("CanGetAttention");
	TCHAR const* const UpdateTargetPoint = TEXT("UpdateTargetPoint");
	TCHAR const* const FirstTargetPointUpdate = TEXT("FirstTargetPointUpdate");
	TCHAR const* const PlayerHidden = TEXT("PlayerHidden");
	TCHAR const* const IsRotating = TEXT("IsRotating");
	TCHAR const* const PlayerKilled = TEXT("PlayerKilled");
	//***********************GUARD***********************
	
	//Object
	TCHAR const* const ForemanActor = TEXT("ForemanActor"); 
	TCHAR const* const PawnActor = TEXT("PawnActor"); 

	//Bool
	TCHAR const* const Warned = TEXT("Warned");
	TCHAR const* const MoveToWarningTargetPoint = TEXT("MoveToWarningTargetPoint");
	TCHAR const* const LKPReached = TEXT("LKPReached");
	TCHAR const* const StartAssassinate = TEXT("StartAssassinate");
	TCHAR const* const Armed = TEXT("Armed");
	TCHAR const* const AnimKillNotify = TEXT("AnimKillNotify");
	TCHAR const* const SeeingHiding = TEXT("SeeingHiding");
	TCHAR const* const SomeOneScaping = TEXT("SomeOneScaping");
	TCHAR const* const IsChangingPoint = TEXT("IsChangingPoint");
	TCHAR const* const HeardSomethingOnPatrol = TEXT("HeardSomethingOnPatrol");

	//Float
	TCHAR const* const HeardSomethingOnPatrolWaitTime = TEXT("HeardSomethingOnPatrolWaitTime");
	TCHAR const* const LoseInterestWaitTime = TEXT("LoseInterestWaitTime");
	
	//Vector
	TCHAR const* const WarningTargetPointPosition = TEXT("WarningTargetPointPosition");
	TCHAR const* const WarningTargetPointRotation = TEXT("WarningTargetPointRotation");
	//***********************PAWN***********************

	

	//***********************FOREMAN***********************

	//Object
	TCHAR const* const GuardActor = TEXT("GuardActor");

	//Vector
	TCHAR const* const GuardLastKnownPoint = TEXT("GuardLastKnownPoint");
	
	//Float
	TCHAR const* const DistanceToGuard = TEXT("DistanceToGuard");
	
	//Bool
	TCHAR const* const WarnedGuard = TEXT("WarnedGuard");
}