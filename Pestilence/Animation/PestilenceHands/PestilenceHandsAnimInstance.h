// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../Enums/Ability.h"
#include "../../Enums/AbilityState.h"
#include "PestilenceHandsAnimInstance.generated.h"

class UPestilenceAbilityComponent;
class APestilenceCharacter;
class UPestilenceMovementComponent;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceHandsAnimInstance : public UAnimInstance
{
  GENERATED_BODY()

public:
  virtual void NativeInitializeAnimation() override;
  virtual void NativeBeginPlay() override;
  virtual void NativeUpdateAnimation(float DeltaSeconds) override;
  virtual void BeginDestroy() override;
  
  UFUNCTION()
  void OnAbilitySelected(EAbility _Ability);
  UFUNCTION()
  void OnStateChanged(EAbilityState _AbilityState);
  UFUNCTION()
  void OnCrouch(bool IsCrouch); 
  
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Animation")
	void CrouchStarted();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Animation")
	void CrouchEnded();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Animation")
	void OnTranslating(bool _bIsTranslating, UPestilenceMovementComponent* _MovementComponent);
  UFUNCTION()
  void OnTakeDownStarted();
  UFUNCTION()
  void OnStartDeathFall();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Animation")
	void ProjectileAppear();


  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<APestilenceCharacter> Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<UPestilenceAbilityComponent> AbilityComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<UPestilenceMovementComponent> MovementComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability")
  EAbility CurrentAbility;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability")
  EAbilityState AbilityState;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability")
  bool bCanReuseAbility;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	float GroundSpeed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
	bool bShouldMove = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bIsFalling = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bStartCrouch = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bEndCrouch = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bIsRolling = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bDie = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bOpenDoor = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bIsInMinigameDoor = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bIsSuccessMinigameDoor = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bIsFailMinigameDoor = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bIsTranslating = false;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Essential Movement Data")
  bool bTakeDownStarted = false;

  

};
