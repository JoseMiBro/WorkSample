#pragma once
#include "CoreMinimal.h"
#include "PossessionState.h"
#include "TransitionState.generated.h"

class APestilenceCharacter;
class UCameraComponent;

UCLASS()
class PESTILENCE_API UTransitionState : public UPossessionState
{
	GENERATED_BODY()

public:
	void SetPossessingCharacter(const TObjectPtr<APestilenceCharacter>& Character)
	{
		PossessingCharacter = Character;
	}
	
private:
	TObjectPtr<APestilenceCharacter> Owner;
	TObjectPtr<APestilenceCharacter> PossessingCharacter;
	TObjectPtr<UCameraComponent> CameraComponent;
	FVector StartSelfCameraLocation;
	FRotator StartSelfCameraRotation;
	float TransitionTime = 0.f;

	
	virtual void CreateState() override;
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;
};