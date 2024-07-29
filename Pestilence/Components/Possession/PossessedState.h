#pragma once
#include "CoreMinimal.h"
#include "PossessionState.h"
#include "PossessedState.generated.h"

class APestilenceAIController;
class UCameraComponent;
class UPestilenceVisionComponent;
class APestilenceCharacter;

UCLASS()
class PESTILENCE_API UPossessedState : public UPossessionState
{
	GENERATED_BODY()

public:
	const float GetCurrentIntegrity()	const { return CurrentIntegrity; }
	void SetCurrentIntegrity(const float _CurrentIntegrity)	{ CurrentIntegrity = _CurrentIntegrity; }

private:
	FDelegateHandle VisionInputEventCallback;
	FDelegateHandle PossessionInputEventCallback;
	bool bTryingToPossessLookingCharacter = false;
	float PossessionAttemptTime = 0.f;
	float DefaultFov = 0.f;
	UPROPERTY()
	float CurrentIntegrity;
	
	TObjectPtr<UPestilenceVisionComponent> VisionComponent;
	TObjectPtr<APestilenceCharacter> Owner;
	TObjectPtr<APestilenceCharacter> LookingCharacter;

	UPROPERTY()
	TObjectPtr<APlayerCameraManager> PlayerCameraManager;

	UPROPERTY()
	TArray<TObjectPtr<APestilenceAIController>> AIControllerActors;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicHandsMaterial = nullptr;

	virtual void CreateState() override;
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;

	void JumpToTransitionState();
	void ResetValues();

	
};
