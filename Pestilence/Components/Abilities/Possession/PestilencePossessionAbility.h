#pragma once

#include "CoreMinimal.h"
#include "Pestilence/Components/Abilities/PestilenceAbility.h"
#include "PestilencePossessionAbility.generated.h"

UCLASS()
class PESTILENCE_API UPestilencePossessionAbility : public UPestilenceAbility
{
	GENERATED_BODY()

private:
	TObjectPtr<class UPestilencePossessionComponent> PossessionComponent;

	virtual void Initialize_Implementation(UWorld* _World) override;
	virtual void StartPrepare_Implementation(bool& _OutResult) override;
	virtual void EndPrepare_Implementation() override;
	virtual void StartCast_Implementation(bool& _OutResult) override;
	virtual void CancelCast_Implementation() override;
	virtual void StartAbility_Implementation() override;

	virtual void SetNewOwner(APestilenceCharacter* _Owner) override;
};
