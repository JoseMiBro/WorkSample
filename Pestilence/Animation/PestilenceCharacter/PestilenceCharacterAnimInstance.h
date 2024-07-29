// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../Components/AI/AIReactionComponent.h"
#include "../../AI/PestilenceAIController.h"
#include "Pestilence/Enums/Gender.h"
#include "PestilenceCharacterAnimInstance.generated.h"

class APestilenceCharacter;
class UPestilenceMovementComponent;
class APestilenceAIController;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Animation")
	void AssassinationFinished();
	
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Animation")
	void WeaponEquipped();

	UFUNCTION()
	void OnClimbing(bool _bIsClimbing, bool _bAscending);
	
	UFUNCTION()
	void OnUsingDoor(bool _bIsUsingDoor, float _InitialYawRotation ,float _CurrentYawRotation);

	UFUNCTION()
	void OnKillPlayer(bool _bCrouched);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<APestilenceCharacter> Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<UPestilenceMovementComponent> MovementComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	TObjectPtr<UAnimSequence> DefaultWaitingAnimation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	TObjectPtr<UAnimSequence> WaitingAnimation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	TObjectPtr<UAnimSequence> RumourAnimation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	float GroundSpeed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Head Movement Data")
	bool bShouldLookAtPlayer = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Head Movement Data")
	FVector LookAtPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Head Movement Data")
	float MaxMovementAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	bool bShouldMove = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bIsFalling = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bStartAssassinate = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bKillPlayer = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bKillCrouched = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bSomeoneInfected = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bArmed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	bool bIsWaitingOnPatrolPoint = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	bool bIsClimbing = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	bool bAscending = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	bool bIsInRumour = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	bool bIsUsingDoor = false;
	
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  EPestilenceTypeCharacter CharacterType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	EPestilenceStates State;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	EGender Gender;

};
