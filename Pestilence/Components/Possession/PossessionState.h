#pragma once
#include "CoreMinimal.h"
#include "PossessionState.generated.h"

class UPestilencePossessionComponent;

UCLASS()
class PESTILENCE_API UPossessionState : public UObject
{
	GENERATED_BODY()
	
public:
	UPossessionState() = default;
	TObjectPtr<UPestilencePossessionComponent> GetPossessionComponent() { return PossessionComponent; }
	
	bool IsActive() const { return bIsActive; }

	virtual void CreateState();
	virtual void EnterState();
	virtual void UpdateState(float DeltaTime);
	virtual void ExitState();
	
private:
	TObjectPtr<UPestilencePossessionComponent> PossessionComponent;
	bool bIsActive = false;

	void SetPossessionComponent(const TObjectPtr<UPestilencePossessionComponent>& Possession);
	
	friend class UPestilencePossessionComponent;
};
