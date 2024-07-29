// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PestilenceAbilityComponent.generated.h"

class UPestilenceAbility;
class APestilenceCharacter;
class UBaseAbilityState;
class UIdleAbilityState;
class UPrepareAbilityState;
class UCastAbilityState;
class UUseAbilityState;
class USoundBase;
class UPrepareNoIntegrityAbilityState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotEnoughIntegrity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCastStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCastEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrepareStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrepareEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrepareNoIntegrityStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrepareNoIntegrityEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilitySelected, EAbility, _Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityEquipped, EAbility, _Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, EAbilityState, _AbilityState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UPestilenceAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UPestilenceAbilityComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnNotEnoughIntegrity OnNotEnoughIntegrity;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnCastStarted OnCastStarted;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnCastEnded OnCastEnded;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnPrepareStarted OnPrepareStarted;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnPrepareEnded OnPrepareEnded;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnPrepareNoIntegrityStarted OnPrepareNoIntegrityStarted;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnPrepareNoIntegrityEnded OnPrepareNoIntegrityEnded;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnAbilityStarted OnAbilityStarted;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnAbilityEnded OnAbilityEnded;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnAbilitySelected OnAbilitySelected;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnAbilityEquipped OnAbilityEquipped;
	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnStateChanged OnStateChanged;

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void SelectAbility(int _Ability, bool _bPlaySound = true);

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void EquipAbility(int _Ability, bool _bPlaySound = true);

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void SelectAbilityNext();

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void SelectAbilityPrev();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeState(UBaseAbilityState* _NewState);

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void StartCast();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void CancelCast();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void StartPrepare();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void CancelPrepare();

	void BroadcastNotEnoughIntegrity() const;
	void BroadcastCastStarted() const;
  void BroadcastCastEnded() const;
  void BroadcastPrepareStarted() const;
  void BroadcastPrepareEnded() const;
  void BroadcastPrepareNoIntegrityStarted() const;
  void BroadcastPrepareNoIntegrityEnded() const;
	void BroadcastAbilityStarted() const;
	void BroadcastAbilityEnded() const;

  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	UPestilenceAbility* GetCurrentAbility() const;
	int GetCurrentAbilityId() const;
	UBaseAbilityState* GetCurrentState() const;
	UBaseAbilityState* GetIdleState() const;
	UBaseAbilityState* GetPrepareState() const;
	UBaseAbilityState* GetPrepareNoIntegrityState() const;
	UBaseAbilityState* GetCastState() const;
	UBaseAbilityState* GetUseState() const;

	float GetCurrentIntegrity() const; 
	void SetCurrentIntegrity(float _Integrity);
	bool GetIsTryPrepare() const;

	APestilenceCharacter* GetPestilenceOwner();

  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void EnableAbilities(bool _Enable);

	UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
	TObjectPtr<USoundBase> NotEnoughIntegritySound;

	int GetMaxAbilities() const;
	float GetPrepareSuspiciousTime() const;
private:
  UFUNCTION()
  void OnTakePickable(UPickableComponent* Pickable, bool bResult);
  UFUNCTION()
  void OnDropPickable(UPickableComponent* Pickable, bool bLaunched);
	UFUNCTION()
	void Initialize();
	UFUNCTION()
	void OnEndPossess();
	UFUNCTION()
	void OnMaxAbilitiesChanged(int _MaxAbilities);

	int MaxAbilities;

	TArray<TObjectPtr<UPestilenceAbility>> AbilityObjects;
  TObjectPtr<UPestilenceAbility> CurrentAbility;

  UPROPERTY()
	TObjectPtr<UPestilenceAbility> PickableAbility;

	int CurrentAbilityId;

	TObjectPtr<APestilenceCharacter> Owner;

	UPROPERTY()
	TObjectPtr<UBaseAbilityState> CurrentState;
	UPROPERTY()
	TObjectPtr<UIdleAbilityState> IdleState;
	UPROPERTY()
	TObjectPtr<UPrepareAbilityState> PrepareState;
	UPROPERTY()
	TObjectPtr<UPrepareNoIntegrityAbilityState> PrepareNoIntegrityState;
	UPROPERTY()
	TObjectPtr<UCastAbilityState> CastState;
	UPROPERTY()
	TObjectPtr<UUseAbilityState> UseState;

  bool bTryPrepare = false;
	UPROPERTY(BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
	bool bCanUseAbilities = true;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
	float PrepareSuspiciousTime = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
	float ChangeAbilityTime = 0.5f;

	float ChangeAbilityTimer = 0.f;
	bool bChangingAbility = false;
	bool bPlaySoundOnChangeAbility = false;
	int AbilityToChange;

};
