#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FailStateComponent.generated.h"

class UCameraComponent;
class UCurveVector;
class UMaterialParameterCollection;
class UCurveFloat;
class UPestilenceMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFailStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartDeathFall);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PESTILENCE_API UFailStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// ----------- Events -----------
	
	/*UPROPERTY(BlueprintAssignable, Category = "FailState")*/
	
	// ------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	TObjectPtr<USoundBase> FailSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	float LookAtGuardTime = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	float FirstDelay = 1.5f; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	TObjectPtr<UCurveVector> StandingFallPosCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	TObjectPtr<UCurveVector> StandingFallRotCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	TObjectPtr<UCurveVector> StandingFloorRotCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	TObjectPtr<UCurveVector> CrouchedFallPosCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	TObjectPtr<UCurveVector> CrouchedFallRotCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	TObjectPtr<UCurveVector> CrouchedFloorRotCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	TObjectPtr<UCurveFloat> SlowdownCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	float FallDelay = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	float FloorDelay = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	float LoadMenuFailsafeTime = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	float MaxPostProcessOffset = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FailState")
	float SetPostProcessTime = 1.5f;

  UPROPERTY(EditAnywhere, Category = "FailState")
  TObjectPtr<UMaterialParameterCollection> DeadStateParameterCollection;

  UPROPERTY(EditAnywhere, Category = "FailState")
  FName OffsetName;
	
  UPROPERTY(BlueprintAssignable, Category = "FailState")
	FOnFailStart OnFailStart;
  UPROPERTY(BlueprintAssignable, Category = "FailState")
	FOnStartDeathFall OnStartDeathFall;

	UFailStateComponent();

	UFUNCTION(BlueprintCallable)
	bool PlayerDieResponse(const AActor* TheKillerActor);

	UFUNCTION(BlueprintCallable)
	void Reset();

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintCallable)
	void UpdateAllBlackboards();

  UFUNCTION(BlueprintCallable)
  const AActor* GetKillerActor() const { return KillerActor; }

  void StartFallToDeath();
private:
	enum class ECurrentState
	{
		None, LookAtGuard, Delay, FallToDeath, OnFloor, LoadMenu
	};

  APlayerController* GetCurrentPlayerController();
  void StartDeathFall();
  void StartOnFloorState();

	virtual void BeginPlay() override;
	virtual void Activate(bool bReset) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Deactivate() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	void SelectCurves();

	ECurrentState CurrentState = ECurrentState::None;
	bool bIsDead = false;
	const AActor* KillerActor = nullptr;
	UCameraComponent* CameraComponent = nullptr;

	// LookAtGuard
	FRotator TargetRotation;

	// Delay
	float DelayTimer = 0;
	float DelayValue = 0;

	FTransform StartTransform;
	FVector StartRotEuler;

	float FallValue = 0.f;
	float FloorValue = 0.f;

	ECurrentState StateAfterDelay = ECurrentState::None;

	bool bSetPostProcess = false;

	float SetPostProcessTimer = 0.f;

	float LoadMenuFailsafeTimer = 0.f;

	bool bSetSlowdown = false;
	float SlowdownValue;
	float SlowdownTimer = 0.f;

  TObjectPtr<UCurveVector> FallPosCurve;
  TObjectPtr<UCurveVector> FallRotCurve;
  TObjectPtr<UCurveVector> FloorRotCurve;

	bool bIgnorePosX = false;
	bool bIgnorePosY = false;

};