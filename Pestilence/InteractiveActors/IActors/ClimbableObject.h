// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbableObject.generated.h"

class UNavArea;
class UPestilenceNavLinkComponent;
class UInteractableComponent;
class UInteractorComponent;
class UBoxComponent;
class USceneComponent;
class UArrowComponent;
class UPestilenceMovementComponent;

UCLASS()
class PESTILENCE_API AClimbableObject : public AActor
{
	GENERATED_BODY()

public:
	AClimbableObject();
protected:
	virtual void BeginPlay() override;

	/* AI */
	UPROPERTY(EditAnywhere, Category = "AI|NavLinkComponent")
	TObjectPtr<UPestilenceNavLinkComponent> PestilenceNavLinkComponent;

	UPROPERTY(EditAnywhere, Category = "AI|NavArea")
	TSubclassOf<UNavArea> InteractNavArea;

public:
	UFUNCTION()
  void OnInteracted(bool bResult, UInteractorComponent* Sender);

	UFUNCTION()
  void OnPreClimbEnded(bool _bIsTranslating, UPestilenceMovementComponent* _MovementComponent);

	//AI
	UPestilenceNavLinkComponent* GetPestilenceNavLinkComponent() const { return PestilenceNavLinkComponent; }
	
private:
  UPROPERTY(EditAnywhere)
  TObjectPtr<UStaticMeshComponent> Wall;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UBoxComponent> InteractableCollider;
	
	UPROPERTY(EditAnywhere, Category = "Climbable Object")
	TObjectPtr<USceneComponent> EnterPos;
  UPROPERTY(EditAnywhere, Category = "Climbable Object")
  TObjectPtr<UArrowComponent> EnterArrow;
	UPROPERTY(EditAnywhere, Category = "Climbable Object")
	TObjectPtr<USceneComponent> TargetPos;
  UPROPERTY(EditAnywhere, Category = "Climbable Object")
  TObjectPtr<UArrowComponent> TargetArrow;
	
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInteractableComponent> InteractableComponent;

	UPROPERTY(EditAnywhere, Category = "Climbable Object")
	bool bForceCrouch;
  UPROPERTY(EditAnywhere, Category = "Sound")
  TObjectPtr<USoundBase> ClimbSound;
};
