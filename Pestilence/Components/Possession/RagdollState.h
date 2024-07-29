#pragma once
#include "CoreMinimal.h"
#include "PossessionState.h"
#include "UObject/Object.h"
#include "RagdollState.generated.h"
UCLASS()

class PESTILENCE_API URagdollState : public UPossessionState
{
	GENERATED_BODY()
	
	TObjectPtr<class APestilenceCharacter> Owner;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicDissolveMaterials;

	float DissolveLevel = -1.f;
	bool bIsDestroyed = false;
	
	virtual void CreateState() override;
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
};