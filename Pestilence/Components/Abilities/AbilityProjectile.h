// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class USphereComponent;
class APestilenceCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileSet, const FVector&, _Pos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLifeTimeEnded);

UCLASS()
class PESTILENCE_API AAbilityProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
  AAbilityProjectile();

	void Initialize(const FVector& _NewPos, const FRotator& _NewRotator);

  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
  FOnProjectileSet OnProjectileSet;
  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnLifeTimeEnded OnLifeTimeEnded;

  bool IsActive();

  const UProjectileMovementComponent* GetProjectileComponent() const;
  float GetLifeTime() const;

  void SetProjectileOwner(APestilenceCharacter* _Owner);

  float GetMaxFloorAngle() const;

  void SetVisibility(bool _bVisible);

  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
  void EndLife();

protected:
  virtual void BeginPlay() override;

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION()
	virtual void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
	void Hide();


  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UStaticMeshComponent> Ball;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<USphereComponent> SphereCollision;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence | Ability", meta = (AllowPrivateAccess = "true"))
  float LifeTime = 5.f;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence | Ability", meta = (AllowPrivateAccess = "true"))
  float MaxFloorAngle = 30.f;

	float LifeTimer = 0.f;

  bool bIsActive = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
  TObjectPtr<USoundBase> ImpactSound;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
  TObjectPtr<USoundBase> BounceSound;

  TObjectPtr<APestilenceCharacter> Owner;
  
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;
  
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  FName ScaleInDepthName = TEXT("Scale In Depth");
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  float ScaleInDepthHandValue = 0.1f;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  float ScaleInDepthThrownValue = 1.f;
  
	UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
	FVector MaxRotationSpeed = FVector(-100.f, -500.f, 0.f);
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
	FVector MinRotationSpeed = FVector(0.f, -100.f, 0.f);

  FVector RotationSpeed;
};
