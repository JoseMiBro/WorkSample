// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pestilence/Enums/HidingSpotType.h"
#include "IA_HidingSpot.generated.h"

class UInteractableComponent;
class UInteractorComponent;
class UBoxComponent;
class USceneComponent;
class UArrowComponent;
class UPestilenceMovementComponent;

UCLASS()
class PESTILENCE_API AIA_HidingSpot : public AActor
{
	GENERATED_BODY()

public:
	AIA_HidingSpot();
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHidingSpotType Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngleLimit = -1.f;

	UFUNCTION(BlueprintCallable)
	const UBoxComponent* GetPassByArea1() const;
	
	UFUNCTION(BlueprintCallable)
	const UBoxComponent* GetPassByArea2() const;

  UFUNCTION()
  void OnInteracted(bool bResult, UInteractorComponent* Sender);

	void OnPreEnterHideout(UPestilenceMovementComponent* _MovementComponent);

	FVector GetEnterPos() const;
	FRotator GetEnterDir() const;
	float GetCapsuleHalfHeight() const;
	float GetCapsuleRadius() const;
private:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<USceneComponent> EnterTarget;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UArrowComponent> EnterArrow;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<USceneComponent> ExitTarget;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UArrowComponent> ExitArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> InteractableCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> PassByArea1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> PassByArea2;
	
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInteractableComponent> InteractableComponent;

	UPROPERTY(EditAnywhere, Category = "Hiding Spot", Meta = (ClampMin = "1"))
	float CapsuleHalfHeight;

	UPROPERTY(EditAnywhere, Category = "Hiding Spot", Meta = (ClampMin = "1"))
	float CapsuleRadius;

	UPROPERTY(EditAnywhere, Category = "Sound")
  TObjectPtr<USoundBase> HideSound;

};
