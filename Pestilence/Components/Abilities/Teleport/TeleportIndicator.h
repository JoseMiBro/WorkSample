// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportIndicator.generated.h"

class USceneComponent;
class UNiagaraComponent;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeEnded);

UCLASS()
class PESTILENCE_API ATeleportIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
  FOnTimeEnded OnTimeEnded;

  void Initialize(const FVector& _NewPos);
  void Hide();
  
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
  float GetTeleportTimer() const;
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
  bool IsActive() const;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<USoundBase> IdleSound;
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<USoundBase> FadeSound;
private:

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<USceneComponent> RootScene;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> NiagaraParticles;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> NiagaraSplash;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UAudioComponent> IdleAudioComponent;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UAudioComponent> FadeAudioComponent;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  float TeleportTime = 10.f;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  float AudioFadeOutTime = 0.1f;

  float TeleportTimer = 0.f;

  bool bIsActive = false;
};
