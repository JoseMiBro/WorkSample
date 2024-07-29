// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceMovementComponent.h"
#include <Pestilence/Characters/PestilenceCharacter.h> 	 	
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h" 	
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "../../Enums/SuspiciousActionType.h"
#include "../Perception/SuspiciousActionSenderComponent.h"
#include <Pestilence/Enums/HidingSpotType.h> 	 	
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "Pestilence/InteractiveActors/IActors/IA_HidingSpot.h"
#include "../Possession/PestilencePossessionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "../Abilities/PestilenceAbilityComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Curves/CurveVector.h"
#include "../../InteractiveActors/IActors/ClimbableObject.h"
#include "../Interaction/InteractorComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Pestilence/Components/Perception/PestilenceSightComponent.h"
#include "Pestilence/Core/PestilenceGameUserSettings.h"

void UPestilenceMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCrouchInterp)
		CrouchInterp(DeltaTime);
	if (bIsRolling)
		Roll(DeltaTime);
	if (bIsRollRecover)
		RollRecover(DeltaTime);
	if (bIsRollPenalty)
		RollPenalty(DeltaTime);
	if (bIsClimbing)
		Climb(DeltaTime);
	if (bIsTranslating)
		Translate(DeltaTime);
  if (bIsEnteringHideout)
    Hide(DeltaTime);
  else if (bIsPreEnteringHideout)
    PreHide(DeltaTime);
	if (bIsLeaning)
    Lean(DeltaTime);
	else
    LeanTickCheck();
	if (bJustPossessed)
		JustPossessed(DeltaTime);
	if (Velocity.SizeSquared() == 0.f || IsFalling())
		EndHeadBobbing();
	if (bIsLookingAt)
		LookAt(DeltaTime);
	if (bTakeDown)
		TakeDown(DeltaTime);
}

void UPestilenceMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner<APestilenceCharacter>();

	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
	StartCameraPivotPos = Owner->GetCameraPivotComponent()->GetRelativeLocation();
	DefaultHalfHeight = Owner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	DefaultRadius = Owner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	DefaultCrouchedHalfHeight = GetCrouchedHalfHeight();
	MaxWalkSpeed = RunSpeed;
  MaxWalkSpeedCrouched = CrouchSpeed;
  DefaultMaxAcceleration = MaxAcceleration;

	if (Owner->GetPestilencePossessionComponent()->IsPossessed())
		bAlwaysCheckFloor = 1;
	Owner->GetPestilencePossessionComponent()->OnBeginPossessEvent().AddDynamic(this, &UPestilenceMovementComponent::OnBeginPossess);
	Owner->GetPestilencePossessionComponent()->OnPossessed.AddDynamic(this, &UPestilenceMovementComponent::OnPossessed);


	float MinCurveTime, MaxCurveTime;

	if (ensureMsgf(CrouchCurve, TEXT("Crouch Curve is not assigned"))) 
	{
		CrouchCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
		CrouchTime = MaxCurveTime - MinCurveTime;
  }
	if (ensureMsgf(LeanCurve, TEXT("Lean Curve is not assigned")))
	{
		LeanCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
		LeanTime = MaxCurveTime - MinCurveTime;
  }
  if (ensureMsgf(TranslationCurve, TEXT("Translation Curve is not assigned")))
  {
		TranslationCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
		TranslateTime = MaxCurveTime - MinCurveTime;
  }
  if (ensureMsgf(ClimbPosCurve, TEXT("Climb Pos Curve is not assigned")))
  {
    ClimbPosCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    ClimbTime = MaxCurveTime - MinCurveTime;
  }
  if (ensureMsgf(ClimbCameraCurve, TEXT("Climb Camera Curve is not assigned")))
  {
    ClimbCameraCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    ensureMsgf(ClimbTime == MaxCurveTime - MinCurveTime, TEXT("Climb Curves have different times"));
  }
  if (ensureMsgf(PreHideCurve, TEXT("PreHide Curve is not assigned")))
  {
		PreHideCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
		PreHideTime = MaxCurveTime - MinCurveTime;
  }
  if (ensureMsgf(ClimbCameraCurve, TEXT("PreHide Pos Camera Curve is not assigned")))
  {
		PreHidePosCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    ensureMsgf(PreHideTime == MaxCurveTime - MinCurveTime, TEXT("PreHide Curves have different times"));
  }
  if (ensureMsgf(HideCurve, TEXT("Hide Curve is not assigned")))
  {
    HideCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    HideTime = MaxCurveTime - MinCurveTime;
  }
  if (ensureMsgf(ClimbCameraCurve, TEXT("Hide Camera Curve is not assigned")))
  {
		HideCameraCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    ensureMsgf(HideTime == MaxCurveTime - MinCurveTime, TEXT("Hide Curves have different times"));
  }
	if (ensureMsgf(RollCurve, TEXT("Roll Curve is not assigned")))
	{
		RollCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
		RollTime = MaxCurveTime - MinCurveTime;
  }
  if (ensureMsgf(RollCameraHeightCurve, TEXT("Roll Camera Height Curve is not assigned")))
  {
		RollCameraHeightCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    ensureMsgf(RollTime == MaxCurveTime - MinCurveTime, TEXT("Roll Curves have different times"));
  }
  if (ensureMsgf(ClimbCameraCurve, TEXT("Roll Camera Pitch Curve is not assigned")))
  {
		RollCameraPitchCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    ensureMsgf(RollTime == MaxCurveTime - MinCurveTime, TEXT("Roll Curves have different times"));
  }
  if (ensureMsgf(TakeDownCurve, TEXT("TakeDown Curve is not assigned")))
  {
    TakeDownCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
		TakeDownTime = MaxCurveTime - MinCurveTime;
  }
	

  APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
  DefaultMaxYaw = PlayerCameraManager->ViewYawMax;
  DefaultMinYaw = PlayerCameraManager->ViewYawMin;
  DefaultMaxPitch = PlayerCameraManager->ViewPitchMax;
  DefaultMinPitch = PlayerCameraManager->ViewPitchMin;

	PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}

void UPestilenceMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Owner->GetPestilencePossessionComponent()->OnBeginPossessEvent().RemoveDynamic(this, &UPestilenceMovementComponent::OnBeginPossess);
  Owner->GetPestilencePossessionComponent()->OnPossessed.RemoveDynamic(this, &UPestilenceMovementComponent::OnPossessed);
  
	Super::EndPlay(EndPlayReason);
}

void UPestilenceMovementComponent::Move(const FVector2D _MovementVector)
{
	if (!bCanMove || IsRolling()) return;

	else if (bTryWalk && !bIsWalking && !bWantsToCrouch)
		Walk();

	FVector MovementVector = GetOwner()->GetActorForwardVector() * _MovementVector.Y + GetOwner()->GetActorRightVector() * _MovementVector.X;
	AddInputVector(MovementVector, false);
	LastMovementDir = _MovementVector;

  if (Velocity.SizeSquared() > 0.f)
    StartHeadBobbing();

  LastSpeedSquared = Velocity.SizeSquared();
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
	//if (!_MovementVector.IsNearlyZero(GenerateSoundByMoveTolerance))
	//	Owner->GenerateSound();
}

void UPestilenceMovementComponent::Look(const FVector2D _LookAxisVector)
{
	if (!bCanLook) return;

	float LookX = _LookAxisVector.X;
	float LookY = _LookAxisVector.Y;

	GetPawnOwner()->AddControllerYawInput(LookX);
	GetPawnOwner()->AddControllerPitchInput(LookY);

	/*
	if (!bLookLimited) return;

  FRotator NewRot = GetPawnOwner()->GetControlRotation();
	NewRot.Yaw = NewRot.Yaw > 180.f ? NewRot.Yaw - 360.f : NewRot.Yaw;
	NewRot.Yaw = NewRot.Yaw < -180.f ? NewRot.Yaw + 360.f : NewRot.Yaw;
	NewRot.Pitch = NewRot.Pitch > 180.f ? NewRot.Pitch - 360.f : NewRot.Pitch;
	NewRot.Pitch = NewRot.Pitch < -180.f ? NewRot.Pitch + 360.f : NewRot.Pitch;
	if (FMath::Abs(NewRot.Yaw - StartLimitRot.Yaw) > LookAngleLimit)
		NewRot.Yaw = FMath::Sign(NewRot.Yaw - StartLimitRot.Yaw) * LookAngleLimit + StartLimitRot.Yaw;
	if (FMath::Abs(NewRot.Pitch - StartLimitRot.Pitch) > LookAngleLimit)
		NewRot.Pitch = FMath::Sign(NewRot.Pitch - StartLimitRot.Pitch) * LookAngleLimit + StartLimitRot.Pitch;

	Owner->GetController()->SetControlRotation(NewRot);
	*/
}

void UPestilenceMovementComponent::TryLean(float _LeanAmount)
{
	DesiredLeanAmount = _LeanAmount;
	StartLean(DesiredLeanAmount);
}

void UPestilenceMovementComponent::StartLean(float _LeanAmount, bool _bIsEmergencyLean /* = false */)
{
	if (TargetLeanAmount == _LeanAmount || LeanAmount == _LeanAmount) return;
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;


	StartLeanAmount = LeanAmount;
	TargetLeanAmount = _LeanAmount;

	LeanTimer = 0.f;

	if (TargetLeanAmount != 0)
	{
		float CheckLeanResult = CheckLean(TargetLeanAmount);
		if (CheckLeanResult != 0)
		{
			CheckLeanResult = CheckLeanResult == -1 ? 0 : CheckLeanResult;
			TargetLeanAmount = CheckLeanResult * FMath::Sign(TargetLeanAmount);
		}
	}
	if (TargetLeanAmount == StartLeanAmount) return;

  bIsLeaning = true;
  Owner->GetFirstPersonCameraComponent()->bUsePawnControlRotation = 0;
	AuxLeanTime = FMath::Abs((TargetLeanAmount - StartLeanAmount) * LeanTime);
	AuxLeanTime = _bIsEmergencyLean ? AuxLeanTime * EmergencyLeanTimeMultiplier : AuxLeanTime;
}

void UPestilenceMovementComponent::Lean(float _DeltaTime)
{
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;

	LeanTimer += _DeltaTime;
	LeanTimer = LeanTimer >= AuxLeanTime ? AuxLeanTime : LeanTimer;

  FRotator CameraRot = Owner->GetFirstPersonCameraComponent()->GetRelativeRotation();
  CameraRot.Pitch = GetController()->GetControlRotation().Pitch;
  Owner->GetFirstPersonCameraComponent()->SetRelativeRotation(CameraRot);

	if (AuxLeanTime != 0)
  {
    float CurveValue = LeanCurve->GetFloatValue((1 / AuxLeanTime) * LeanTimer * LeanTime);
		LeanAmount = FMath::Lerp(StartLeanAmount, TargetLeanAmount, CurveValue);
		LeanAmount = FMath::Abs(LeanAmount) > 1 ? 1 * FMath::Sign(LeanAmount) : LeanAmount;
		LeanAmount = FMath::Abs(LeanAmount) < 0.01f ? 0 : LeanAmount;

		float NewAngle = LeanAmount * MaxLeanAngle;
		FRotator NewRotation = FRotator::MakeFromEuler(FVector(NewAngle, 0, 0));

		float NewPosY = LeanAmount * MaxLeanLength;
		FVector NewPos = StartCameraPivotPos;
		NewPos.Y = NewPosY;

		if (FMath::Abs(StartLeanAmount) > FMath::Abs(TargetLeanAmount) || !CheckLean(LeanAmount))
		{
			Owner->GetCameraPivotComponent()->SetRelativeRotation(NewRotation);
			Owner->GetCameraPivotComponent()->SetRelativeLocation(NewPos, true);
		}
		else
		{
			TargetLeanAmount = LeanAmount;
			StopLean();
		}
	}
	if (LeanTimer >= AuxLeanTime)
		StopLean();
}

void UPestilenceMovementComponent::StopLean()
{
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;

	bIsLeaning = false;
  Owner->GetFirstPersonCameraComponent()->bUsePawnControlRotation = LeanAmount == 0 && !bIsRolling ? 1 : 0;
}

void UPestilenceMovementComponent::TryCrouch()
{
	if (!bCanMove || bIsHidden) return;


	bTryCrouch = !bTryCrouch;
	if (IsRolling()) return;

	StartCrouchInterp(bTryCrouch);

	if (bTryCrouch)
		UGameplayStatics::PlaySound2D(GetWorld(), CrouchSound, 1.f, 1.f, 0.f);
	else
		UGameplayStatics::PlaySound2D(GetWorld(), UncrouchSound, 1.f, 1.f, 0.f);
}

void UPestilenceMovementComponent::StartCrouchInterp(bool _bShouldCrouch)
{
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;

	bCrouchInterp = true;
	bWantsToCrouch = true;
	StopWalk();
	if(_bShouldCrouch)
		OnCrouch.Broadcast(bWantsToCrouch);

	StartHalfHeight = Owner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	TargetHalfHeight = _bShouldCrouch ? DefaultCrouchedHalfHeight : DefaultHalfHeight;
	SetCrouchedHalfHeight(StartHalfHeight);
	CrouchTimer = 0.f;
}

void UPestilenceMovementComponent::CrouchInterp(float _DeltaTime)
{
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;

	CrouchTimer = bIsRolling ? CrouchTimer + (_DeltaTime * (CrouchTime / RollCrouchTime)): CrouchTimer + _DeltaTime;
	CrouchTimer = CrouchTimer >= CrouchTime ? CrouchTime : CrouchTimer;

	float CurveValue = CrouchCurve->GetFloatValue(CrouchTimer);
	float NewHalfHeight = FMath::Lerp(StartHalfHeight, TargetHalfHeight, CurveValue);
	Owner->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);

	if (CrouchTimer >= CrouchTime)
		EndCrouch();
}

void UPestilenceMovementComponent::EndCrouch()
{
	if (StartHalfHeight < TargetHalfHeight)
	{
		bWantsToCrouch = false;
		OnCrouch.Broadcast(bWantsToCrouch);

  }
  if (bTryWalk)
    Walk();
	bCrouchInterp = false;
}

//void UPestilenceMovementComponent::TryUnCrouch()
//{
//	if (!bCanMove) return;
//
//	bTryCrouch = false;
//	if (IsRolling()) return;
//
//	bWantsToCrouch = false;
//}

void UPestilenceMovementComponent::TryWalk(bool _bTryWalk)
{
  bTryWalk = _bTryWalk;
  if (bTryWalk)
    Walk();
  else
    StopWalk();
}

void UPestilenceMovementComponent::TryWalkToggle()
{ 
	TryWalk(!bTryWalk);
}

void UPestilenceMovementComponent::Walk()
{
	if (!bCanMove) return;

	bIsWalking = true;
	MaxWalkSpeedCrouched *= WalkMultiplier;
	MaxWalkSpeed *= WalkMultiplier;
}

void UPestilenceMovementComponent::StopWalk()
{
	bIsWalking = false;
	if (bIsRollPenalty || !Owner->GetPestilencePossessionComponent()->IsPossessed()) return;

  MaxWalkSpeedCrouched = CrouchSpeed;
  MaxWalkSpeed = RunSpeed;
}

void UPestilenceMovementComponent::StartRoll(FVector _RollDirection)
{
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
	if (!bCanMove || bIsRolling || bIsRollRecover || bIsHidden || Owner->GetPestilenceAbilityComponent()->GetCurrentState() != Owner->GetPestilenceAbilityComponent()->GetIdleState()) return;


	if (!bWantsToCrouch)
		StartCrouchInterp(true);
  bIsRolling = true;

	// Pausa la animación de andar para que no suenen pasos, si acaba habiendo animación de rodar habrá que quitar esta linea
	Owner->GetMesh()->bPauseAnims = true;

	UGameplayStatics::PlaySound2D(GetWorld(), RollSound, 1.f, 1.f, 0.f);
	//MaxWalkSpeedCrouched = RollSpeed;
	MaxAcceleration = RollAcceleration;
	RollTimer = 0;
	//RollDirection = _RollDirection;
	//if (RollDirection.X == 0 && RollDirection.Y == 0)
	//{
	RollDirection = GetOwner()->GetActorForwardVector();
  //}
  Owner->GetFirstPersonCameraComponent()->bUsePawnControlRotation = 0;

  EnableLook(false);
	EndHeadBobbing();
}

void UPestilenceMovementComponent::Roll(float _DeltaTime)
{
	RollTimer += _DeltaTime;

	AddInputVector(RollDirection, false);

	float CurveValue = RollCurve->GetFloatValue(RollTimer);
	MaxWalkSpeedCrouched = FMath::Lerp(RunSpeed, RollSpeed, CurveValue);

	CurveValue = RollCameraHeightCurve->GetFloatValue(RollTimer);
	float NewCameraHeight = FMath::Lerp(RollCameraMinHeight + StartCameraPivotPos.Z, StartCameraPivotPos.Z, CurveValue);
	FVector CameraLocation = Owner->GetCameraPivotComponent()->GetRelativeLocation();
	CameraLocation.Z = NewCameraHeight;
	Owner->GetCameraPivotComponent()->SetRelativeLocation(CameraLocation);

  CurveValue = RollCameraPitchCurve->GetFloatValue(RollTimer);
  float NewCameraPitch = FMath::Lerp(RollCameraMaxPitch, RollCameraMinPitch, CurveValue);
  FRotator CameraRotation = Owner->GetCameraPivotComponent()->GetRelativeRotation();
  CameraRotation.Pitch = NewCameraPitch;
  Owner->GetCameraPivotComponent()->SetRelativeRotation(CameraRotation);

	if (RollTimer > RollTime)
	{
		StopRoll();
	}
}

void UPestilenceMovementComponent::StopRoll()
{
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
	if (!Owner->GetPestilencePossessionComponent()->IsPossessed()) return;

	MaxWalkSpeedCrouched = CrouchSpeed;
  bIsRolling = false;

	// Despausa la animaci�n de andar, si acaba habiendo animaci�n de rodar habr� que quitar esta linea
	Owner->GetMesh()->bPauseAnims = false;

	RollTimer = 0;

	if (!bTryCrouch && bCanMove)
		StartCrouchInterp(bTryCrouch);

	bIsRollRecover = true;
	bIsRollPenalty= true;

  MaxWalkSpeed = RollPenaltySpeed;
  MaxWalkSpeed = bIsWalking ? MaxWalkSpeed * WalkMultiplier : MaxWalkSpeed;
  MaxWalkSpeedCrouched = RollPenaltySpeedCrouched;
	MaxWalkSpeedCrouched = bIsWalking ? MaxWalkSpeedCrouched * WalkMultiplier : MaxWalkSpeedCrouched;
  MaxAcceleration = DefaultMaxAcceleration;

  Owner->GetFirstPersonCameraComponent()->bUsePawnControlRotation = LeanAmount == 0 && !bIsRolling ? 1 : 0;
  EnableLook(true);
}

void UPestilenceMovementComponent::RollRecover(float _DeltaTime)
{
	RollRecoveryTimer += _DeltaTime;

	if (RollRecoveryTimer > RollRecoveryTime)
	{
		RollRecoveryTimer = 0;
		bIsRollRecover = false;
	}
}

void UPestilenceMovementComponent::RollPenalty(float _DeltaTime)
{
	RollPenaltyTimer += _DeltaTime;

	MaxWalkSpeed = FMath::Lerp(RollPenaltySpeed, RunSpeed, RollPenaltyTimer / RollPenaltyTime);
	MaxWalkSpeed = bIsWalking ? MaxWalkSpeed * WalkMultiplier : MaxWalkSpeed;
  MaxWalkSpeedCrouched = FMath::Lerp(RollPenaltySpeedCrouched, CrouchSpeed, RollPenaltyTimer / RollPenaltyTime);
  MaxWalkSpeedCrouched = bIsWalking ? MaxWalkSpeedCrouched * WalkMultiplier : MaxWalkSpeedCrouched;
	if (RollPenaltyTimer > RollPenaltyTime)
	{
		RollPenaltyTimer = 0;
		bIsRollPenalty = false;
    MaxWalkSpeed = RunSpeed;
    MaxWalkSpeed = bIsWalking ? MaxWalkSpeed * WalkMultiplier : MaxWalkSpeed;
    MaxWalkSpeedCrouched = CrouchSpeed;
    MaxWalkSpeedCrouched = bIsWalking ? MaxWalkSpeedCrouched * WalkMultiplier : MaxWalkSpeedCrouched;
	}
}

void UPestilenceMovementComponent::EnableMovement(bool _bCanMove)
{
	bCanMove = _bCanMove;

	if (!Owner) return;
	if (!_bCanMove)
	{
		if(bIsRolling)
			StopRoll();
    if(bIsWalking)
			StopWalk();
	}
	else if (bTryWalk)
		Walk();

}

void UPestilenceMovementComponent::EnableLook(bool _bCanLook)
{
	bCanLook = _bCanLook;
}

void UPestilenceMovementComponent::LimitLook(float _LimitAngle)
{
  APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
  PlayerCameraManager->ViewYawMax = GetOwner()->GetActorRotation().Yaw + _LimitAngle;
  PlayerCameraManager->ViewYawMin = PlayerCameraManager->ViewYawMax - 2 * _LimitAngle;
  PlayerCameraManager->ViewPitchMax = Owner->GetFirstPersonCameraComponent()->GetComponentRotation().Pitch + _LimitAngle;
  PlayerCameraManager->ViewPitchMin = PlayerCameraManager->ViewPitchMax - 2 * _LimitAngle;

	/*
  bLookLimited = true;
  LookAngleLimit = _LimitAngle;
  StartLimitRot = GetPawnOwner()->GetControlRotation();
  StartLimitRot.Yaw = StartLimitRot.Yaw > 180.f ? StartLimitRot.Yaw - 360.f : StartLimitRot.Yaw;
  StartLimitRot.Yaw = StartLimitRot.Yaw < -180.f ? StartLimitRot.Yaw + 360.f : StartLimitRot.Yaw;
  StartLimitRot.Pitch = StartLimitRot.Pitch > 180.f ? StartLimitRot.Pitch - 360.f : StartLimitRot.Pitch;
  StartLimitRot.Pitch = StartLimitRot.Pitch < -180.f ? StartLimitRot.Pitch + 360.f : StartLimitRot.Pitch;
	*/
}

void UPestilenceMovementComponent::UnlimitLook()
{
  APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
  PlayerCameraManager->ViewYawMax = DefaultMaxYaw;
  PlayerCameraManager->ViewYawMin = DefaultMinYaw;
  PlayerCameraManager->ViewPitchMax = DefaultMaxPitch;
  PlayerCameraManager->ViewPitchMin = DefaultMinPitch;
}

void UPestilenceMovementComponent::StartTranslation(const FVector& _TargetPos, const FRotator& _TargetRot, float _TargetHalfHeight, float _TargetRadius)
{
  bIsTranslating = true;
  OnTranslating.Broadcast(true, this);
  EnableMovement(false);
  EnableLook(false);

  StartTranslationPos = GetOwner()->GetActorLocation();
  StartTranslationRot = GetOwner()->GetActorRotation();
	StartTranslationRot.Pitch = Owner->GetFirstPersonCameraComponent()->GetComponentRotation().Pitch;
  TargetTranslationPos = _TargetPos;
  TargetTranslationRot = _TargetRot;

  StartHalfHeight = Owner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
  StartRadius = Owner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
  TargetHalfHeight = _TargetHalfHeight > 0.f ? _TargetHalfHeight : bWantsToCrouch ? DefaultCrouchedHalfHeight : DefaultHalfHeight;
	TargetRadius = _TargetRadius > 0.f ? _TargetRadius : StartRadius;

	TargetTranslationPos.Z += TargetHalfHeight;

  TranslateTimer = 0;

  if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
  Owner->GetPestilenceAbilityComponent()->EnableAbilities(false);
  if (Owner->GetPestilencePossessionComponent()->IsPossessed())
		Owner->GetInteractorComponent()->Enable(false);
}

void UPestilenceMovementComponent::StartTakeDown(const FVector& _TargetPos)
{
  bTakeDown = true;

  StartTranslationPos = GetOwner()->GetActorLocation();
  TargetTranslationPos = _TargetPos;

  TakeDownTimer = 0;
}

void UPestilenceMovementComponent::StartClimbing(const FVector& _TargetPos, bool _bForceCrouch, bool _bClimbingDown)
{
  bIsClimbing = true;
	bClimbingDown = _bClimbingDown;
  OnTranslating.Broadcast(true, this);
	EnableMovement(false);
	EnableLook(false);

	if (_bForceCrouch)
	{ 
		bWantsToCrouch = true;
		bTryCrouch = true;

    OnCrouch.Broadcast(bWantsToCrouch);
	}

	StartClimbingTransform = GetOwner()->GetTransform();
  StartClimbingRotEuler = GetOwner()->GetActorRotation().Euler();
  TargetTranslationPos = _TargetPos;
	TargetTranslationPos.Z += bTryCrouch ? DefaultCrouchedHalfHeight : DefaultHalfHeight;
	ClimbTargetRelativePos = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetTransform(), TargetTranslationPos);
	ClimbTimer = 0;
	bool bAscending = TargetTranslationPos.Z > StartClimbingTransform.GetLocation().Z;
	OnClimbing.Broadcast(true, bAscending);

	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
  Owner->GetSuspiciousActionSenderComponent()->SubmitSuspiciousAction(GetOwner(), ESuspiciousActionType::Climb);
  if (Owner->GetPestilencePossessionComponent()->IsPossessed())
		Owner->GetInteractorComponent()->Enable(false);
}

void UPestilenceMovementComponent::PreEnterHideout(const FVector& _TargetPos, const FRotator& _TargetRot, float _TargetHalfHeight, float _TargetRadius, AIA_HidingSpot* _HidingSpot)
{
  if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
  bWantsToCrouch = true;
  bIsPreEnteringHideout = true;
  OnTranslating.Broadcast(true, this);
  EnableMovement(false);
  EnableLook(false);

  StartTranslationPos = GetOwner()->GetActorLocation();
  StartTranslationRot = GetOwner()->GetActorRotation();
	StartTranslationRot.Pitch = Owner->GetFirstPersonCameraComponent()->GetComponentRotation().Pitch;
  TargetTranslationPos = _TargetPos;
  TargetTranslationRot = _TargetRot;
  StartHalfHeight = Owner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
  StartRadius = Owner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
  TargetHalfHeight = _TargetHalfHeight;
  TargetRadius = _TargetRadius;
  HideTimer = 0;
  HidingSpot = _HidingSpot;
  TargetTranslationPos.Z += TargetHalfHeight;

  Owner->GetPestilenceAbilityComponent()->EnableAbilities(false);
  if (Owner->GetPestilencePossessionComponent()->IsPossessed())
		Owner->GetInteractorComponent()->Enable(false);
	
}

void UPestilenceMovementComponent::EnterHideout(const FVector& _TargetPos, EHidingSpotType _HiddenType)
{
  if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
  bWantsToCrouch = true;
  bIsEnteringHideout = true;
  OnTranslating.Broadcast(true, this);
	bIsHidden = true;
	OnHide.Broadcast(bIsHidden);
	EnableMovement(false);
  EnableLook(false);
  UGameplayStatics::PlaySound2D(GetWorld(), HideSound);

	StartTranslationPos = GetOwner()->GetActorLocation();
	StartHidingRotEuler = GetOwner()->GetActorRotation().Euler();
  TargetTranslationPos = _TargetPos;
  TargetTranslationPos.Z += TargetHalfHeight;
	HiddenType = _HiddenType;
	HideTimer = 0;

	UpdateAllBlackboards(false);
}

void UPestilenceMovementComponent::ExitHideout(const FVector& _TargetPos, const FRotator& _TargetRot)
{
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;

	bIsPreEnteringHideout = true;
  bIsHidden = false;
  OnHide.Broadcast(bIsHidden);
  OnTranslating.Broadcast(true, this);
	EnableMovement(false);
  EnableLook(false);
  UGameplayStatics::PlaySound2D(GetWorld(), EndHideSound);

	StartTranslationPos = GetOwner()->GetActorLocation();
	StartTranslationRot = GetOwner()->GetActorRotation();
	StartTranslationRot.Pitch = Owner->GetFirstPersonCameraComponent()->GetComponentRotation().Pitch;
	TargetTranslationPos = _TargetPos;
	TargetTranslationRot = _TargetRot;

	StartHalfHeight = Owner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	StartRadius = Owner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	if (bWantsToCrouch)
	{
		TargetHalfHeight = DefaultCrouchedHalfHeight;
    TargetRadius = DefaultRadius;

    OnCrouch.Broadcast(bWantsToCrouch);
	}
	else
	{
		TargetHalfHeight = DefaultHalfHeight;
		TargetRadius = DefaultRadius;
  }
  TargetTranslationPos.Z += TargetHalfHeight;
	HideTimer = 0;
	HidingSpot = nullptr;
  UpdateAllBlackboards(true);
  Owner->GetPestilenceAbilityComponent()->EnableAbilities(false);
  if (Owner->GetPestilencePossessionComponent()->IsPossessed())
		Owner->GetInteractorComponent()->Enable(false);
}

void UPestilenceMovementComponent::CancelTranslations()
{
	if (bIsTranslating)
		EndTranslation();
	else if (bIsClimbing)
		StopClimbing();
	else if (bIsPreEnteringHideout || bIsEnteringHideout)
		EndHide();
}

void UPestilenceMovementComponent::StartLookAt(const FVector& _LookAt, float _LookAtTime)
{
	LookAtTimer = 0.f;
	LookAtTime = _LookAtTime;
	bIsLookingAt = true;
	
  StartLookAtRot = GetOwner()->GetActorRotation();
  StartLookAtRot.Pitch = Owner->GetFirstPersonCameraComponent()->GetComponentRotation().Pitch;
	FVector CurrentPos = GetOwner()->GetActorLocation();
	if (bWantsToCrouch)
		GetStandingPos(CurrentPos);
  TargetLookAtRot = (_LookAt - CurrentPos).Rotation();
}

void UPestilenceMovementComponent::StartUsingDoor(bool _IsUsingDoor, float _InitialYawRotation, float _CurrentYawRotation)
{
	OnUsingDoor.Broadcast(_IsUsingDoor, _InitialYawRotation, _CurrentYawRotation);
}

void UPestilenceMovementComponent::UpdateAllBlackboards(bool test)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),  APestilenceCharacter::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		const TObjectPtr<APestilenceCharacter> PestilenceCharacter = Cast<APestilenceCharacter>(FoundActor);
		if (!IsValid(PestilenceCharacter)) continue;
		
		TObjectPtr<APestilenceAIController> AIController = PestilenceCharacter->GetControllerAI();
		if(!IsValid(AIController)) continue;

		TObjectPtr<UBlackboardComponent> BlackboardComponent = AIController->GetBlackboard();
		if(!IsValid(BlackboardComponent)) continue;

    BlackboardComponent->SetValueAsBool(Keys::PlayerHidden, bIsHidden);
    bool bSeeingPlayer = BlackboardComponent->GetValueAsBool(Keys::SeeingPlayer);
    bool bSeeingHiding = BlackboardComponent->GetValueAsBool(Keys::SeeingHiding);
    BlackboardComponent->SetValueAsBool(Keys::SeeingPlayer, bSeeingHiding && !bIsHidden);

		if (AIController->GetCurrentState() == EPestilenceStates::Chase)
		{
			BlackboardComponent->SetValueAsBool(Keys::SeeingHiding, bSeeingPlayer && bIsHidden);
		}
		if(test)
		{
			AActor* PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(),0)->GetPawn();
			if(IsValid(PlayerPawn))
			{
				for(AActor* Char : AIController->FindComponentByClass<UPestilenceSightComponent>()->ActorsOnSight)
				{
					if(Char == PlayerPawn)
					{
						AIController->FindComponentByClass<UPestilenceSightComponent>()->bOnSight = true;
						BlackboardComponent->SetValueAsBool(Keys::SeeingPlayer, true);

						break;
					}
				}
			}
		}
		else
		{
			AIController->FindComponentByClass<UPestilenceSightComponent>()->bOnSight = false;
			BlackboardComponent->SetValueAsBool(Keys::SeeingPlayer, false);

		}
	}
}

void UPestilenceMovementComponent::OnBeginPossess()
{
	TryLean(0.f);
}

void UPestilenceMovementComponent::OnPossessed()
{
	/*
  SetGroundMovementMode(EMovementMode::MOVE_Walking); 
	
	FVector Location = GetOwner()->GetActorLocation();
	Location.Z += 101.f;
	GetOwner()->SetActorLocation(Location);
	*/

	bJustPossessed = true;
	UnlimitLook();
	bAlwaysCheckFloor = 1;
}

void UPestilenceMovementComponent::JustPossessed(float _DeltaTime)
{
	JustPossessedTimer += _DeltaTime;
	bJustPossessed = JustPossessedTimer >= JustPossessedTime;

	if (!bJustPossessed)
	{
		MaxWalkSpeed = RunSpeed;
		SetGroundMovementMode(EMovementMode::MOVE_Walking);
	}
}

void UPestilenceMovementComponent::LeanTickCheck()
{
	if (LeanAmount != 0)
	{
		if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
		if (!GetController())
		{
			DesiredLeanAmount = 0;
			StartLean(0);
			return;
		}
    FRotator CameraRot = Owner->GetFirstPersonCameraComponent()->GetRelativeRotation();
    CameraRot.Pitch = GetController()->GetControlRotation().Pitch;
		Owner->GetFirstPersonCameraComponent()->SetRelativeRotation(CameraRot);

		float CheckLeanResult = CheckLean(TargetLeanAmount);
		if (CheckLeanResult != 0)
		{
			CheckLeanResult = CheckLeanResult == -1 ? 0 : CheckLeanResult;
			StartLean(CheckLeanResult * FMath::Sign(TargetLeanAmount), true);
		}
		else
			StartLean(DesiredLeanAmount);
	}
	else if (DesiredLeanAmount != 0)
		StartLean(DesiredLeanAmount);
}

float UPestilenceMovementComponent::CheckLean(float _LeanAmount)
{
	// Los comentarios son porque creo que sería mejor hacer para que se use el radio de la capsula para calcular esto, pero tendria que subir la velocidad de Lean si se está volviendo por colisión.
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return false;

  float CurveValue = LeanCurve->GetFloatValue(FMath::Abs(_LeanAmount) * LeanTime);
  float CheckLeanAmount = FMath::Lerp(StartLeanAmount, TargetLeanAmount, CurveValue);

	float NewAngle = CheckLeanAmount * MaxLeanAngle;
	FRotator NewRotation = FRotator::MakeFromEuler(FVector(NewAngle, 0, 0));
	NewRotation = UKismetMathLibrary::TransformRotation(GetOwner()->GetActorTransform(), NewRotation);

	float NewPosY = CheckLeanAmount * MaxLeanLength;
	FVector NewPos = StartCameraPivotPos;
	NewPos.Y = NewPosY;
	NewPos = UKismetMathLibrary::TransformLocation(GetOwner()->GetActorTransform(), NewPos);

	FVector CameraPos = Owner->GetFirstPersonCameraComponent()->GetRelativeLocation();

	FHitResult Hit;

	FTransform CheckTransform = FTransform(NewRotation, NewPos); 
	FVector Start = UKismetMathLibrary::TransformLocation(GetOwner()->GetActorTransform(), CameraPos);
	float CameraHalfWidth = FMath::Tan(FMath::DegreesToRadians(Owner->GetFirstPersonCameraComponent()->FieldOfView) * 0.5f) * LeanCameraDepth;
	CameraPos.Y = CheckLeanAmount * CameraHalfWidth;
	// CameraPos.Y = CheckLeanAmount * DefaultRadius;
  FVector End = UKismetMathLibrary::TransformLocation(CheckTransform, CameraPos);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams);

	// Hit2 es lo mismo pero unas unidades delante
  FHitResult Hit2;
  NewPos = StartCameraPivotPos;
  NewPos.Y = NewPosY;
  NewPos.X += LeanForwardCheck;
  NewPos = UKismetMathLibrary::TransformLocation(GetOwner()->GetActorTransform(), NewPos);

  CameraPos = Owner->GetFirstPersonCameraComponent()->GetRelativeLocation();
  CameraPos.X += LeanForwardCheck;

  CheckTransform = FTransform(NewRotation, NewPos);
  Start = UKismetMathLibrary::TransformLocation(GetOwner()->GetActorTransform(), CameraPos);
  CameraPos.Y = CheckLeanAmount * CameraHalfWidth;
  CameraPos.X -= LeanForwardCheck;
  End = UKismetMathLibrary::TransformLocation(CheckTransform, CameraPos);

  bool bHit2 = GetWorld()->LineTraceSingleByChannel(Hit2, Start, End, ECC_Visibility, CollisionParams);
	float NewLeanAmount = 0.f;
	if (bHit || bHit2)
	{
		float Distance = FVector::Distance(Start, End);
		if(!bHit2)
      NewLeanAmount = (Hit.Distance - CameraHalfWidth) / (Distance - CameraHalfWidth);
		else if(!bHit)
      NewLeanAmount = (Hit2.Distance - CameraHalfWidth) / (Distance - CameraHalfWidth);
		else
		{
			float LeanAmount1 = (Hit.Distance - CameraHalfWidth) / (Distance - CameraHalfWidth);
			float LeanAmount2 = (Hit2.Distance - CameraHalfWidth) / (Distance - CameraHalfWidth);
			NewLeanAmount = LeanAmount1 < LeanAmount2 ? LeanAmount1 : LeanAmount2;
    }
    NewLeanAmount = NewLeanAmount > 0.05 ? NewLeanAmount : -1;
  }
	return NewLeanAmount;
}

void UPestilenceMovementComponent::Translate(float _DeltaTime)
{
  TranslateTimer += _DeltaTime;
	TranslateTimer = TranslateTimer >= TranslateTime ? TranslateTime : TranslateTimer;

  float CurveValue = TranslationCurve->GetFloatValue(TranslateTimer);
  FVector NewPos = FMath::Lerp(StartTranslationPos, TargetTranslationPos, CurveValue);
  GetOwner()->SetActorLocation(NewPos);

  FRotator NewRot = FMath::Lerp(StartTranslationRot, TargetTranslationRot, CurveValue);

  if (Owner->GetController())
    Owner->GetController()->SetControlRotation(NewRot);
  else
    GetOwner()->SetActorRotation(NewRot);
  FRotator NewCameraRot = FRotator::ZeroRotator;
  NewCameraRot.Pitch = NewRot.Pitch;
  Owner->GetFirstPersonCameraComponent()->SetRelativeRotation(NewCameraRot);

	if (StartRadius != TargetRadius)
	{
		float NewRadius = FMath::Lerp(StartRadius, TargetRadius, CurveValue);
		Owner->GetCapsuleComponent()->SetCapsuleRadius(NewRadius);
	}

	if (StartHalfHeight != TargetHalfHeight)
	{
		float NewHalfHeight = FMath::Lerp(StartHalfHeight, TargetHalfHeight, CurveValue);
		Owner->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);
	}

  if (TranslateTimer >= TranslateTime)
		EndTranslation();
}

void UPestilenceMovementComponent::EndTranslation()
{
  bIsTranslating = false;
  bUseControllerDesiredRotation = 0;
	if (ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner")) && (TargetHalfHeight == DefaultHalfHeight || TargetHalfHeight == DefaultCrouchedHalfHeight))
	{
    Owner->GetPestilenceAbilityComponent()->EnableAbilities(true);
    if (Owner->GetPestilencePossessionComponent()->IsPossessed())
			Owner->GetInteractorComponent()->Enable(true);
	}
  OnTranslating.Broadcast(false, this);

  EnableMovement(true);
  EnableLook(true);
  if (TargetHalfHeight == DefaultHalfHeight && bTryCrouch)
    StartCrouchInterp(bTryCrouch);
}

void UPestilenceMovementComponent::TakeDown(float _DeltaTime)
{
  TakeDownTimer += _DeltaTime;
	TakeDownTimer = TakeDownTimer >= TakeDownTime ? TakeDownTime : TakeDownTimer;

  float CurveValue = TakeDownCurve->GetFloatValue(TakeDownTimer);
  FVector NewPos = FMath::Lerp(StartTranslationPos, TargetTranslationPos, CurveValue);
  GetOwner()->SetActorLocation(NewPos);

  if (TakeDownTimer >= TakeDownTime)
		EndTakeDown();
}

void UPestilenceMovementComponent::EndTakeDown()
{
  bTakeDown = false;
}

void UPestilenceMovementComponent::Climb(float _DeltaTime)
{
	ClimbTimer += _DeltaTime;
	ClimbTimer = ClimbTimer >= ClimbTime ? ClimbTime : ClimbTimer;

	FVector CurvePosValue;
	if (bClimbingDown)
  {
    CurvePosValue = ClimbPosCurve->GetVectorValue(ClimbTime - ClimbTimer);
		CurvePosValue.X = FMath::Abs(1 - CurvePosValue.X);
		CurvePosValue.Y = FMath::Abs(1 - CurvePosValue.Y);
		CurvePosValue.Z = FMath::Abs(1 - CurvePosValue.Z);
	}
	else
	{
    CurvePosValue = ClimbPosCurve->GetVectorValue(ClimbTimer);
	}
  float NewPosX = FMath::Lerp(0, ClimbTargetRelativePos.X, CurvePosValue.X);
  float NewPosY = CurvePosValue.Y;
  float NewPosZ = FMath::Lerp(0, ClimbTargetRelativePos.Z, CurvePosValue.Z);
	GetOwner()->SetActorLocation(UKismetMathLibrary::TransformLocation(StartClimbingTransform, FVector(NewPosX, NewPosY, NewPosZ)));

  FRotator NewRot = FRotator::MakeFromEuler(StartClimbingRotEuler + ClimbCameraCurve->GetVectorValue(ClimbTimer));
  if (Owner->GetController())
    Owner->GetController()->SetControlRotation(NewRot);
  else
    GetOwner()->SetActorRotation(NewRot);

	if (ClimbTimer >= ClimbTime)
		StopClimbing();
}

void UPestilenceMovementComponent::StopClimbing()
{
  bIsClimbing = false;
  OnTranslating.Broadcast(false, this);

	EnableMovement(true);
  EnableLook(true);
	OnClimbing.Broadcast(false, false);
	if(bTryCrouch)
		StartCrouchInterp(bTryCrouch);	
  if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
  Owner->GetPestilenceAbilityComponent()->EnableAbilities(true);
	if(Owner->GetPestilencePossessionComponent()->IsPossessed())
		Owner->GetInteractorComponent()->Enable(true);
}

void UPestilenceMovementComponent::PreHide(float _DeltaTime)
{
  HideTimer += _DeltaTime;
  HideTimer = HideTimer >= PreHideTime ? PreHideTime : HideTimer;

	float CurveValue = PreHideCurve->GetFloatValue(HideTimer);

  FVector NewPos;
	if (HidingSpot)
  {
    FVector CurvePosValue = PreHidePosCurve->GetVectorValue(HideTimer);
    float NewPosX = FMath::Lerp(StartTranslationPos.X, TargetTranslationPos.X, CurvePosValue.X);
    float NewPosY = FMath::Lerp(StartTranslationPos.Y, TargetTranslationPos.Y, CurvePosValue.Y);
    float NewPosZ = FMath::Lerp(StartTranslationPos.Z, TargetTranslationPos.Z, CurvePosValue.Z);
		NewPos = FVector(NewPosX, NewPosY, NewPosZ);
	}
	else
	{
		NewPos = FMath::Lerp(StartTranslationPos, TargetTranslationPos, CurveValue);
	}
  GetOwner()->SetActorLocation(NewPos);

  FRotator NewRot = FMath::Lerp(StartTranslationRot, TargetTranslationRot, CurveValue);

  if (Owner->GetController())
    Owner->GetController()->SetControlRotation(NewRot);
  else
    GetOwner()->SetActorRotation(NewRot);
	FRotator NewCameraRot = FRotator::ZeroRotator;
	NewCameraRot.Pitch = NewRot.Pitch;
	Owner->GetFirstPersonCameraComponent()->SetRelativeRotation(NewCameraRot);

  float NewRadius = FMath::Lerp(StartRadius, TargetRadius, CurveValue);
  Owner->GetCapsuleComponent()->SetCapsuleRadius(NewRadius);

  float NewHalfHeight = FMath::Lerp(StartHalfHeight, TargetHalfHeight, CurveValue);
  Owner->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);

  if (HideTimer >= PreHideTime)
    EndPreHide();
}

void UPestilenceMovementComponent::EndPreHide()
{
  bIsPreEnteringHideout = false;
	if (HidingSpot)
		HidingSpot->OnPreEnterHideout(this);
	else
		EndHide();
}

void UPestilenceMovementComponent::Hide(float _DeltaTime)
{
	if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;

	HideTimer += _DeltaTime;
	HideTimer = HideTimer >= HideTime ? HideTime : HideTimer;

  float CurveValue = HideCurve->GetFloatValue(HideTimer);
	
	FVector NewPos = FMath::Lerp(StartTranslationPos, TargetTranslationPos, CurveValue);
	GetOwner()->SetActorLocation(NewPos);
	
  FRotator NewRot = FRotator::MakeFromEuler(StartHidingRotEuler + HideCameraCurve->GetVectorValue(HideTimer));

  if (Owner->GetController())
    Owner->GetController()->SetControlRotation(NewRot);
  else
    GetOwner()->SetActorRotation(NewRot);

	if (HideTimer >= HideTime)
		EndHide();
}

void UPestilenceMovementComponent::EndHide()
{
  if (!ensureMsgf(Owner, TEXT("Movement Component has no pestilence character owner"))) return;
  bIsPreEnteringHideout = false;
  bIsEnteringHideout = false;
  OnTranslating.Broadcast(false, this);
  bTryCrouch = true;
	MaxAcceleration = DefaultMaxAcceleration;
	if (bIsHidden)
	{
		if (HidingSpot->AngleLimit != -1.f)
			LimitLook(HidingSpot->AngleLimit);
		switch (HiddenType)
		{
		case EHidingSpotType::Fixed:
			EnableMovement(false);
			EnableLook(false);
			break;
		case EHidingSpotType::CameraMovement:
			EnableMovement(false);
			EnableLook(true);
			break;
		case EHidingSpotType::FreeMovement:
			EnableMovement(true);
			EnableLook(true);
			break;
		}
	}
	else
	{
		UnlimitLook();
		EnableMovement(true);
		EnableLook(true);
  }
  Owner->GetPestilenceAbilityComponent()->EnableAbilities(true);
  if (Owner->GetPestilencePossessionComponent()->IsPossessed())
		Owner->GetInteractorComponent()->Enable(true);
}

void UPestilenceMovementComponent::LookAt(float _DeltaTime)
{
  LookAtTimer += _DeltaTime;
	LookAtTimer = LookAtTimer >= LookAtTime ? LookAtTime : LookAtTimer;

  FRotator NewRot = FMath::Lerp(StartLookAtRot, TargetLookAtRot, LookAtTimer / LookAtTime);

  if (Owner->GetPestilencePossessionComponent()->IsPossessed() && Owner->GetController())
    Owner->GetController()->SetControlRotation(NewRot);
  else
    GetOwner()->SetActorRotation(NewRot);
  FRotator NewCameraRot = FRotator::ZeroRotator;
  NewCameraRot.Pitch = NewRot.Pitch;
  Owner->GetFirstPersonCameraComponent()->SetRelativeRotation(NewCameraRot);

   if (LookAtTimer >= LookAtTime)
		EndLookAt();
}

void UPestilenceMovementComponent::EndLookAt()
{
  bIsLookingAt = false;
}

void UPestilenceMovementComponent::StartHeadBobbing()
{
	float SpeedSquared = Velocity.SizeSquared();

	if (IsFalling() || (LastSpeedSquared == SpeedSquared && CurrentHeadBobbing) || 
		(PestilenceGameUserSettings && !PestilenceGameUserSettings->bHeadBobbing)) return;

	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (CurrentHeadBobbing)
		PlayerCameraManager->StopAllInstancesOfCameraShake(HeadBobbingShake, false);
	CurrentHeadBobbing = PlayerCameraManager->StartCameraShake(HeadBobbingShake, SpeedSquared / (RunSpeed * RunSpeed));
	

}

void UPestilenceMovementComponent::EndHeadBobbing()
{
  if (!CurrentHeadBobbing) return;

  UGameplayStatics::GetPlayerCameraManager(this, 0)->StopAllInstancesOfCameraShake(HeadBobbingShake, false);
	CurrentHeadBobbing = nullptr;
}

const float UPestilenceMovementComponent::GetLeanAmount() const
{
	return LeanAmount;
}

const float UPestilenceMovementComponent::GetTargetLeanAmount() const
{
	return TargetLeanAmount;
}

const bool UPestilenceMovementComponent::GetIsWalking() const
{
	return bIsWalking;
}

const bool UPestilenceMovementComponent::IsRolling() const
{
	return bIsRolling;
}

const bool UPestilenceMovementComponent::IsRollRecover() const
{
	return bIsRollRecover;
}

const bool UPestilenceMovementComponent::IsRollPenalty() const
{
	return bIsRollPenalty;
}


const float UPestilenceMovementComponent::GetHearingCrouchMaxRange() const
{
	return HearingCrouchMaxRange;
}

const float UPestilenceMovementComponent::GetHearingMediumMaxRange() const
{
	return HearingMediumMaxRange;
}

const float UPestilenceMovementComponent::GetHearingFastMaxRange() const
{
  return HearingFastMaxRange;
}

float UPestilenceMovementComponent::GetHalfHeight() const
{
	return Owner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
}

float UPestilenceMovementComponent::GetMediumSpeed() const
{
	return MediumSpeed;
}

float UPestilenceMovementComponent::GetFastSpeed() const
{
	return FastSpeed;
}

void UPestilenceMovementComponent::EnterHideoutTeleport(const FVector& _TargetPos, const FRotator& _TargetDir, float _TargetHalfHeight, float _TargetRadius, EHidingSpotType _HiddenType, AIA_HidingSpot* _HidingSpot)
{
	bTryCrouch = true;
	bWantsToCrouch = true;
  bIsHidden = true;
  OnHide.Broadcast(bIsHidden);
  HiddenType = _HiddenType;
  HidingSpot = _HidingSpot;
	StopMovementImmediately();
	
  Owner->GetCapsuleComponent()->SetCapsuleRadius(_TargetRadius);
	SetCrouchedHalfHeight(_TargetHalfHeight);
  Owner->GetCapsuleComponent()->SetCapsuleHalfHeight(_TargetHalfHeight);

  GetOwner()->SetActorLocation(FVector(_TargetPos.X, _TargetPos.Y, _TargetPos.Z + _TargetHalfHeight));
	Owner->bUseControllerRotationYaw = 0;
  GetOwner()->SetActorRotation(_TargetDir);
	Owner->bUseControllerRotationYaw = 1;

  EndHide();
}

void UPestilenceMovementComponent::ExitHideoutTeleport()
{
  bIsHidden = false;
  OnHide.Broadcast(bIsHidden);
  HidingSpot = nullptr;
  Owner->GetCapsuleComponent()->SetCapsuleRadius(DefaultRadius);
  Owner->GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCrouchedHalfHeight);
  if (bWantsToCrouch)
  {
    Owner->GetCapsuleComponent()->SetCapsuleRadius(DefaultRadius);
    SetCrouchedHalfHeight(DefaultCrouchedHalfHeight);

    OnCrouch.Broadcast(bWantsToCrouch);
  }
  else
  {
    Owner->GetCapsuleComponent()->SetCapsuleRadius(DefaultRadius);
    Owner->GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultHalfHeight);
  }
  UpdateAllBlackboards(true);
  EndHide();
}

void UPestilenceMovementComponent::GetStandingPos(FVector& _CurrentPos) const
{
	_CurrentPos.Z = _CurrentPos.Z - DefaultCrouchedHalfHeight + DefaultHalfHeight;
}

const bool UPestilenceMovementComponent::IsHidden() const
{
	return bIsHidden;
}

const EHidingSpotType UPestilenceMovementComponent::GetHidingSpotType() const
{
	return HiddenType;
}

const AIA_HidingSpot* UPestilenceMovementComponent::GetHidingSpot() const
{
	return HidingSpot;
}