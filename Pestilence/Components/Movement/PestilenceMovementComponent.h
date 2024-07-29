// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include <Pestilence/Enums/HidingSpotType.h>
#include "PestilenceMovementComponent.generated.h"

class AIA_HidingSpot;
class UInputAction;
class APestilenceCharacter;
class UCameraShakeBase;
class ULegacyCameraShake;
class UCurveFloat;
class UCurveVector;
class AClimbableObject;
class UPestilenceGameUserSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCrouch, bool, IsCrouch);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHide, bool, IsHidden);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUsingDoor, bool, _IsUsingDoor, float, _InitialYawRotation, float, _CurrentYawRotation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTranslating, bool, _bIsTranslating, UPestilenceMovementComponent*, _MovementComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClimbing, bool, _bIsClimbing, bool, _bAscending);

UENUM()
enum class EHeadBobbingType : uint8
{
    None
	, Soft
  , Medium
  , Strong
};

UCLASS(Blueprintable)
class PESTILENCE_API UPestilenceMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void Move(const FVector2D _MovementVector);
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void Look(const FVector2D _LookAxisVector);

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void TryLean(float _LeanAmount);
	void StartLean(float _LeanAmount, bool _bIsEmergencyLean = false);
	void Lean(float _DeltaTime);
	void StopLean();

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void TryCrouch();
	void StartCrouchInterp(bool _bShouldCrouch);
	void CrouchInterp(float _DeltaTime);
	void EndCrouch();
	//void TryUnCrouch();

  UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void TryWalk(bool _bTryWalk);
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void TryWalkToggle();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void Walk();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void StopWalk();

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void StartRoll(FVector _RollDirection);
	void Roll(float _DeltaTime);
	void StopRoll();
	void RollRecover(float _DeltaTime);
	void RollPenalty(float _DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	const float GetLeanAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	const float GetTargetLeanAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	const bool GetIsWalking() const;
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	const bool IsRolling() const;
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	const bool IsRollRecover() const;
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	const bool IsRollPenalty() const;
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	const bool IsHidden() const;
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	const EHidingSpotType GetHidingSpotType() const;
	UFUNCTION(BlueprintCallable,Category="Pestilence|Movement")
	const AIA_HidingSpot* GetHidingSpot() const;
	
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void EnableMovement(bool _bCanMove);
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void EnableLook(bool _bCanLook);
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void LimitLook(float _LimitAngle);
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void UnlimitLook();
	
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void StartTranslation(const FVector& _TargetPos, const FRotator& _TargetRot, float _TargetHalfHeight = -1.f, float _TargetRadius = -1.f);
	
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void StartTakeDown(const FVector& _TargetPos);

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void StartClimbing(const FVector& _TargetPos, bool _bForceCrouch, bool _bClimbingDown);
	
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void PreEnterHideout(const FVector& _TargetPos, const FRotator& _TargetRot, float _TargetHalfHeight, float _TargetRadius, AIA_HidingSpot* _HidingSpot);
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void EnterHideout(const FVector& _TargetPos, EHidingSpotType _HiddenType);
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void ExitHideout(const FVector& _TargetPos, const FRotator& _TargetDir);

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void CancelTranslations();

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void StartLookAt(const FVector& _LookAt, float _LookAtTime);

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Movement")
	void StartUsingDoor(bool _IsUsingDoor, float _InitialYawRotation, float _CurrentYawRotation);

  void UpdateAllBlackboards(bool test);
	const float GetHearingCrouchMaxRange() const;
  const float GetHearingMediumMaxRange() const;
  const float GetHearingFastMaxRange() const;

	float GetHalfHeight() const;

	float GetMediumSpeed() const;
	float GetFastSpeed() const; 

	void EnterHideoutTeleport(const FVector& _TargetPos, const FRotator& _TargetDir, float _TargetHalfHeight, float _TargetRadius, EHidingSpotType _HiddenType, AIA_HidingSpot* _HidingSpot);
	void ExitHideoutTeleport();

	void GetStandingPos(FVector& _CurrentPos) const;

  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Movement")
	FOnCrouch OnCrouch;
  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Movement")
	FOnHide OnHide;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Movement")
	FOnTranslating OnTranslating;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Movement")
	FOnClimbing OnClimbing;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Movement")
	FOnUsingDoor OnUsingDoor;

  UPROPERTY(EditAnywhere, Category = "Sound")
  TObjectPtr<USoundBase> RunStepSound;
  UPROPERTY(EditAnywhere, Category = "Sound")
  TObjectPtr<USoundBase> CrouchStepSound;
  UPROPERTY(EditAnywhere, Category = "Sound")
  TObjectPtr<USoundBase> WalkStepSound;
private:
	UFUNCTION()
	void OnBeginPossess();
	UFUNCTION()
	void OnPossessed();

	void JustPossessed(float _DeltaTime);

	void LeanTickCheck();
	float CheckLean(float _LeanAmount);

  void Translate(float _DeltaTime);
  void EndTranslation();

  void TakeDown(float _DeltaTime);
  void EndTakeDown();

  void Climb(float _DeltaTime);
	void StopClimbing();
	
  void PreHide(float _DeltaTime);
	void EndPreHide();
  void Hide(float _DeltaTime);
  void EndHide();

	void LookAt(float _DeltaTime);
	void EndLookAt();

	void StartHeadBobbing();
public:
	void EndHeadBobbing();
private:
	
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float MediumSpeed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float FastSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float RunSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float WalkMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float CrouchSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float RollPenaltySpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float RollPenaltySpeedCrouched = 150.f;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float RollSpeed = 2400.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float RollAcceleration = 10000.f;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float EmergencyLeanTimeMultiplier = 0.1f;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float LeanForwardCheck = 20.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float MaxLeanAngle = 5.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float MaxLeanLength = 50.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float LeanCameraDepth = 65.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float RollCrouchTime = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float RollRecoveryTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float RollPenaltyTime = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float RollCameraMinHeight = -30.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
  float RollCameraMinPitch = 0.f;

  UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
  float RollCameraMaxPitch = -90.f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	float JustPossessedTime= 0.5f;
	
	
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	TObjectPtr<UCurveFloat> CrouchCurve;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	TObjectPtr<UCurveFloat> LeanCurve;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	TObjectPtr<UCurveFloat> TranslationCurve;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	TObjectPtr<UCurveVector> ClimbPosCurve;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	TObjectPtr<UCurveVector> ClimbCameraCurve;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	TObjectPtr<UCurveFloat> PreHideCurve;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	TObjectPtr<UCurveVector> PreHidePosCurve;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	TObjectPtr<UCurveFloat> HideCurve;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
	TObjectPtr<UCurveVector> HideCameraCurve;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
  TObjectPtr<UCurveFloat> RollCurve;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
  TObjectPtr<UCurveFloat> RollCameraHeightCurve;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
  TObjectPtr<UCurveFloat> RollCameraPitchCurve;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Movement")
  TObjectPtr<UCurveFloat> TakeDownCurve;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> CrouchSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> UncrouchSound;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> RollSound;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> HideSound;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> EndHideSound;
	
  UPROPERTY(EditAnywhere, Category = "Pestilence|Camera")
	TSubclassOf<UCameraShakeBase> HeadBobbingShake;

	TObjectPtr<UPestilenceGameUserSettings> PestilenceGameUserSettings;

  float CrouchTime;
  float RollTime;
  float TranslateTime;
  float ClimbTime;
  float PreHideTime;
  float HideTime;
  float LeanTime;
  float TakeDownTime;

	float CrouchTimer = 0.f;

	float RollTimer = 0.f;
	float RollRecoveryTimer = 0.f;
	float RollPenaltyTimer = 0.f;

	float TranslateTimer = 0.f;
	float ClimbTimer = 0.f;
	float TakeDownTimer = 0.f;
	
	float HideTimer = 0.f;

	float LeanTimer = 0.f;
	float AuxLeanTime = 0.f;

	float JustPossessedTimer = 0.f;

	float LookAtTime;
	float LookAtTimer = 0.f;

	float StartLeanAmount = 0.f;
	float LeanAmount = 0.f;
	float TargetLeanAmount = 0.f;
	float DesiredLeanAmount = 0.f;

	float DefaultHalfHeight;
	float DefaultRadius;
	float StartHalfHeight;
	float StartRadius;
	float TargetHalfHeight;
	float TargetRadius;
	float DefaultCrouchedHalfHeight;
	float DefaultMaxAcceleration;
	float DefaultCameraHeight;

	FVector RollDirection = FVector::ZeroVector;

	FVector StartCameraPivotPos;

  FVector StartTranslationPos;
  FVector TargetTranslationPos;
  FRotator StartTranslationRot;
  FRotator TargetTranslationRot;

	FTransform StartClimbingTransform;
  FVector ClimbTargetRelativePos;
  FVector StartClimbingRotEuler;

	FVector StartHidingRotEuler;

	FRotator StartLookAtRot;
	FRotator TargetLookAtRot;

	FVector2D LastMovementDir;

	bool bIsWalking = false;
	bool bCrouchInterp = false;
	bool bIsRolling = false;
	bool bIsRollRecover = false;
	bool bIsRollPenalty = false;
	bool bTryCrouch = false;
	bool bTryWalk = false;
	bool bCanMove = true;
	bool bCanLook = true;
	bool bIsTranslating = false;
	bool bIsClimbing = false;
	bool bClimbingDown = false;
	bool bIsLeaning = false;
	bool bIsPreEnteringHideout = false;
	bool bIsEnteringHideout = false;
	bool bIsHidden = false;
	bool bJustPossessed = false;
	bool bIsLookingAt = false;
	bool bInLockpick = false;
	bool bTakeDown = false;

	float DefaultMaxYaw;
	float DefaultMinYaw;
	float DefaultMaxPitch;
	float DefaultMinPitch;

	EHidingSpotType HiddenType;
	TObjectPtr<AIA_HidingSpot> HidingSpot;
	TObjectPtr<AClimbableObject> ClimbableObject;

	TObjectPtr<UCameraShakeBase> CurrentHeadBobbing = nullptr;

	float LastSpeedSquared = 0.f;

	TObjectPtr<APestilenceCharacter> Owner;

	UPROPERTY(EditAnywhere, Category="Pestilence Hearing")
	float GenerateSoundByMoveTolerance = 0.01f;
	public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pestilence Hearing")
	float HearingFastMaxRange = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pestilence Hearing")
	float HearingMediumMaxRange = 125.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pestilence Hearing")
	float HearingCrouchMaxRange = 0.f;
};