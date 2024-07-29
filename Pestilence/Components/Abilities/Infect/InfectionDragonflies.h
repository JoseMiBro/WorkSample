// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InfectionDragonflies.generated.h"

class UNiagaraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetReached);

UCLASS()
class PESTILENCE_API AInfectionDragonflies : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
  AInfectionDragonflies();

  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnTargetReached OnTargetReached;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  void StartFlying(const FVector& _StartPos, const AActor* _Target);
  void Hide();
	bool IsFlying() const;
private:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<USceneComponent> RootScene;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> NiagaraParticles;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
  float Speed = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
  float MinDistance = 10.f;

	const AActor* Target;

	bool bIsFlying = false;
};
