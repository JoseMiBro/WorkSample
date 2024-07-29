// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../../Enums/AbilityState.h"
#include "BaseAbilityState.generated.h"

class UPestilenceAbilityComponent;
class UPestilenceAbility;

UCLASS()
class PESTILENCE_API UBaseAbilityState : public UObject
{
	GENERATED_BODY()

public:
	TObjectPtr<UPestilenceAbilityComponent> GetAbilityComponent();

	bool IsActive() const { return bIsActive; }

	virtual void CreateState();
	virtual void EnterState();
	virtual void UpdateState(float _DeltaTime);
	virtual void ExitState();
	virtual void StartCast();
	virtual void CancelCast();
	virtual void StartPrepare();
	virtual void CancelPrepare();
	virtual bool CanSelectAbility();

	void SetAbilityComponent(const TObjectPtr<UPestilenceAbilityComponent>& _AbilityComponent);
	EAbilityState GetStateType() const;

protected:
  EAbilityState StateType;

private:
	TObjectPtr<UPestilenceAbilityComponent> AbilityComponent;
	bool bIsActive = false;
};
