// Fill out your copyright notice in the Description page of Project Settings.


#include "SightCalculator.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Components/AI/PestilenceAIComponent.h"

USightCalculator::USightCalculator()
{
	// Init values test
	PvSprint = 3.f;
	PvStandar = 2.f;
	PvStealth = 1.f;
	SIn = 1.f;
	SOut = 0.f;
	MIsMoving = 1.f;
	MIsNotMoving = 0.5f;
			
	CloseAngle = 1.f;
	MidAngle = 0.5f;
	FarAngle = 0.f;
		
	CloseDistance = 1.f;
	MidDistance = 0.5f;
	FarDistance = 0.f;
	
	bOnShadow = false;
	VelocityThreshold = 0.2f;
}

float USightCalculator::CalculateAttention_Implementation()
{
	if(!IsValid(PlayerPawn)) return -1;
	if(!IsValid(OwnerPawn)) return -1;
	if(!IsValid(MovementComponent)) return -1;
	
	float MovementState = 0.f;
	float IsMoving = 0.f;
	float InShadow = 0.f;
	
	//Calc Visibility Factor ...
	// State Factor
	if(MovementComponent->IsWalking())
	{
		MovementState = PvSprint;
		
	}else if(MovementComponent->IsCrouching())
	{
		MovementState = PvStealth;
		
	}else
	{
		MovementState = PvStandar;
	}

	// Speed Factor
	if(PlayerPawn->GetVelocity().Length() <= VelocityThreshold)
	{
		IsMoving = MIsNotMoving;
	}
	else
	{
		IsMoving = MIsMoving;
	}

	//Shadow Factor
	if(bOnShadow)
	{
		InShadow = SIn;
		
		// Blackboard
		OwnerController->PestilenceAIComponent =
			Cast<UPestilenceAIComponent>(OwnerController->GetPawn()->GetComponentByClass(UPestilenceAIComponent::StaticClass()));

		if(IsValid(OwnerController->PestilenceAIComponent))
		{
			const int GuardState = OwnerController->GetBlackboard()->GetValueAsEnum(Keys::State);
			if(GuardState == GetTypeHash(EPestilenceStates::Chase))
				InShadow = SOut;
		}
	}
	else
	{
		InShadow = SOut;
	}
	
	// Check Distance Percentage
	const float DistancePercentage = FVector::Distance(
		PlayerPawn->GetActorLocation(),
		OwnerPawn->GetActorLocation()
		) / SightConfig->LoseSightRadius;
	float DistanceRange = 0.f;

	if(DistancePercentage < 0.5f)
	{
		DistanceRange = CloseDistance;
	}else if( 0.5f>= DistancePercentage && DistancePercentage < 1.f)
	{
		DistanceRange =MidDistance;
	}else
	{
		DistanceRange = FarDistance;
	}

	// Check Angle Percentage
	
	const FVector PlayerLoc = PlayerPawn->GetActorLocation();
	const FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(
		OwnerPawn->GetActorLocation(),
		PlayerLoc
		);
	const float Yaw = OwnerPawn->GetActorRotation().Yaw;

	const float AnglePercentage = (Yaw - FMath::Abs(PlayerRot.Yaw)) / SightConfig->PeripheralVisionAngleDegrees;
	float AngleRange = 0.f;
	
	if(AnglePercentage < 0.5f)
	{
		AngleRange = CloseAngle;
	}else if( 0.5f>= AnglePercentage && DistancePercentage < 1.f)
	{
		AngleRange = MidAngle;
	}else
	{
		AngleRange = FarAngle;

	}
	// Calc. Cuality Of Vision
	const float VisionQuality = DistanceRange * AngleRange;
	
	// Calc. Visibility Factor
	const float CheckForNegative = ((MovementState - InShadow) > 0 ? MovementState - InShadow : 0.f);

	return (CheckForNegative * IsMoving) * VisionQuality;
}
