#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PestilencePossessionComponent.generated.h"

class URagdollState;
class UInputAction;
class UPossessionState;
class UPossessedState;
class UTransitionState;
class UFreeState;
class APestilenceCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPossessedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeginPossessEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndPossessEvent);
DECLARE_EVENT_OneParam(UPestilencePossessionComponent, FVisionInputEvent, bool);
DECLARE_EVENT_OneParam(UPestilencePossessionComponent, FPossessionInputEvent, bool);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCloseToTargetDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetReachedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIntegrityChangedDelegate, float, PreviousIntegrity, float, CurrentIntegrity);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UPestilencePossessionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Possession")
	FCloseToTargetDelegate CloseToTargetDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Possession")
	FTargetReachedDelegate TargetReachedDelegate;
    
	UPROPERTY(BlueprintAssignable, Category = "Possession")
	FIntegrityChangedDelegate IntegrityChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Possession")
	FPossessedSignature OnPossessed;

	UPROPERTY(EditAnywhere, Category = "Possession")
	bool bDisableInputOnPossessing = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession")
	float DegradationSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession")
	float MaxIntegrity = 100.f;

	UPROPERTY(EditAnywhere, Category = "Possession")
	bool bDegradationEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession")
	bool bCanBePossessed = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession")
	bool bCanMove = true;
	
  UPROPERTY(EditAnywhere, Category = "Possession")
  FName DissolveControlName;
  UPROPERTY(EditAnywhere, Category = "Possession")
  FName DissolveOffsetName;
	
  UPROPERTY(EditAnywhere, Category = "Possession")
  TObjectPtr<USoundBase> DissolveSound;

	float DissolveSpeed = 0.4f;
	
	UPestilencePossessionComponent();
	
	void SetPossessed(bool bShouldBePossessed);
	bool IsPossessed() const { return bIsPossessed; }

	TObjectPtr<UPossessionState> GetCurrentState() { return CurrentState; }
	TObjectPtr<UPossessedState> GetPossessedState() { return PossessedState; }
	TObjectPtr<UTransitionState> GetTransitionState() { return TransitionState; }
	TObjectPtr<UFreeState> GetFreeState() { return FreeState; }

	UFUNCTION(BlueprintCallable)
	URagdollState* GetRagdollState() { return RagdollState; }

	float GetTimeToPossess() const { return TimeToPossess; }
	float GetTransitionSmoothing() const { return TransitionSmoothing; }
	float GetFovSpeedOnPossess() const { return FovSpeedOnPossess; }

	UFUNCTION(BlueprintCallable, Category = "Possesion")
	float GetCurrentIntegrity() const;
	UFUNCTION(BlueprintCallable, Category = "Possesion")
	void SetCurrentIntegrity(const float _CurrentIntegrity);

	FBeginPossessEvent& OnBeginPossessEvent() { return BeginPossessEvent; }
	FEndPossessEvent& OnEndPossessEvent() { return EndPossessEvent; }
	
	FVisionInputEvent& OnVisionInputEvent() { return VisionInputEvent; }
	FPossessionInputEvent& OnPossessionInputEvent() { return PossessionInputEvent; }
	
	UFUNCTION(BlueprintCallable)
	void SwitchState(UPossessionState* NewState);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartPossession();
	void CancelPossession();
	void StartVision();
	void CancelVision();
	
	APestilenceCharacter* GetPestilenceOwner() { return Owner; }
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = Possession)
	bool bIsPossessed = false;

	UPROPERTY(EditAnywhere, Category = Possession)
	bool bStartPossessed = false;
	
	UPROPERTY(EditAnywhere, Category = Possession)
	float TransitionSmoothing = 0.1f;
	
	UPROPERTY(EditAnywhere, Category = Possession)
	float TimeToPossess = 1.f;

	UPROPERTY(EditAnywhere, Category = Possession)
	float FovSpeedOnPossess = 8.f;
	
private:
	UPROPERTY(BlueprintAssignable, Category = Possession)
	FBeginPossessEvent BeginPossessEvent;
	
	UPROPERTY(BlueprintAssignable, Category = Possession)
	FEndPossessEvent EndPossessEvent;

	FVisionInputEvent VisionInputEvent;
	FPossessionInputEvent PossessionInputEvent;

	UPROPERTY()
	TObjectPtr<UPossessionState> CurrentState;
	UPROPERTY()
	TObjectPtr<UPossessedState> PossessedState;
	UPROPERTY()
	TObjectPtr<UTransitionState> TransitionState;
	UPROPERTY()
	TObjectPtr<UFreeState> FreeState;
	UPROPERTY()
	TObjectPtr<URagdollState> RagdollState;
	
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;
	bool bIsPossessionEnabled = false;

	TObjectPtr<APestilenceCharacter> Owner;
};