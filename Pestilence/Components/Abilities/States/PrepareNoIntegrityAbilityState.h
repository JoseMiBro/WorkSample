// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAbilityState.h"
#include "PrepareNoIntegrityAbilityState.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPrepareNoIntegrityAbilityState : public UBaseAbilityState
{
	GENERATED_BODY()

public:
	virtual void CreateState() override;
	virtual void EnterState() override;
	virtual void UpdateState(float _DeltaTime) override;
	virtual void ExitState() override;
	virtual void StartCast() override;
	virtual void CancelCast() override;
	virtual void StartPrepare() override;
	virtual void CancelPrepare() override;
	virtual bool CanSelectAbility() override;
};
