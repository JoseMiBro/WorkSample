// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PestilenceAbility.h"
#include "PestilenceTeleportAbility.generated.h"

class ATeleportProjectile;
class ATeleportIndicator;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceTeleportAbility : public UPestilenceAbility
{
	GENERATED_BODY()
    
  virtual void Select_Implementation(bool _bPlaySound = true) override;
  virtual void Initialize_Implementation(UWorld* _World) override;
  virtual void StartPrepare_Implementation(bool& _OutResult) override;
  virtual void TickPrepare_Implementation(float _DeltaTime, bool& _OutResult) override;
  virtual void EndPrepare_Implementation() override;
  virtual void StartCast_Implementation(bool& _OutResult) override;
  virtual void TickCast_Implementation(float _DeltaTime, bool& _OutResult) override;
  virtual void EndCast_Implementation(bool& _OutResult) override;
  virtual void CancelCast_Implementation() override;
  virtual void StartAbility_Implementation() override;
  virtual void TickAbility_Implementation(float _DeltaTime) override;
  virtual void EndAbility_Implementation() override;

  virtual void ProjectileAppear() override;
  virtual void UseAbilityNotify() override;
  
	virtual bool GetCanReuse() const override;

  UFUNCTION()
  void OnTeleportSet(const FVector& _Pos);
  UFUNCTION()
  void OnProjectileLifeTimeEnded();

  virtual void SetNewOwner(APestilenceCharacter* _Owner) override; 

  UFUNCTION()
  void OnIntegrityChanged(float PreviousIntegrity, float CurrentIntegrity);

  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TSubclassOf<ATeleportProjectile> TeleportProjectileClass;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TSubclassOf<ATeleportIndicator> TeleportIndicatorClass;

  UPROPERTY()
  TObjectPtr<ATeleportProjectile> TeleportProjectile;
  UPROPERTY()
  TObjectPtr<ATeleportIndicator> TeleportIndicator;

  void ThrowProjectile();
  void StartTeleporting();
  void EndTeleporting();
  UFUNCTION()
  void HideTeleport();

  FVector StartTeleportPos;
  FVector EndTeleportPos;
  FVector TeleportDir;

  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TObjectPtr<USoundBase> TeleportThrownSound;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TObjectPtr<USoundBase> TeleportSound;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> CylinderMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> SphereMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> DecalMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> NoIntegrityCylinderMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> NoIntegritySphereMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> NoIntegrityDecalMaterial;
  
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  float MoveAmount = 400.f;
  
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  float LookAtTime = 0.3f;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  float FadeOutTime = 0.3f;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  float FadeTime = 0.1f;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  float FadeInTime = 0.3f;

  float FadeTimer = 0.f;

  bool bThrowProjectile = false;
  bool bStartingTeleport = false;
  bool bTeleporting = false;
  bool bSkipStartTeleport = false;
  bool bSkipEndTeleport = false;
  bool bFadeOut = false;
  bool bFadeIn = false;

  FVector ProjectileRelativePos;

public:
  bool IsTeleporting() const;

  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
  const ATeleportIndicator* GetTeleportIndicator() const;
};
