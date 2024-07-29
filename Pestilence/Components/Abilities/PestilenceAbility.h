// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Enums/Ability.h"
#include "PestilenceAbility.generated.h"

class APestilenceCharacter;
class USoundBase;
class UAudioComponent;

UCLASS(Blueprintable, BlueprintType)
class PESTILENCE_API UPestilenceAbility : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void Initialize(UWorld* _World);
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
  void Select(bool _bPlaySound = true);
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
  void StartPrepare(bool& _OutResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void TickPrepare(float _DeltaTime, bool& _OutResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void EndPrepare();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void StartCast(bool& _OutResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void TickCast(float _DeltaTime, bool& _OutResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void EndCast(bool& _OutResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void CancelCast();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void StartAbility();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void TickAbility(float _DeltaTime);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pestilence|Ability")
	void EndAbility();
	
	virtual void Initialize_Implementation(UWorld* _World);
	virtual void Select_Implementation(bool _bPlaySound = true);
	virtual void StartPrepare_Implementation(bool& _OutResult);
	virtual void TickPrepare_Implementation(float _DeltaTime, bool& _OutResult);
	virtual void EndPrepare_Implementation();
	virtual void StartCast_Implementation(bool& _OutResult);
	virtual void TickCast_Implementation(float _DeltaTime, bool& _OutResult);
	virtual void EndCast_Implementation(bool& _OutResult);
	virtual void CancelCast_Implementation(); 
	virtual void StartAbility_Implementation();
	virtual void TickAbility_Implementation(float _DeltaTime);
	virtual void EndAbility_Implementation();
	
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
  virtual void ProjectileAppear();

  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	virtual void UseAbilityNotify();

	virtual UWorld* GetWorld() const override;
	virtual void SetNewOwner(APestilenceCharacter* _Owner);

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	APestilenceCharacter* GetOwner() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
	float IntegrityCost = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
	float CastTime = 0.1f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
	float AbilityDuration = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
	float ReuseAbilityDuration = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
	float AudioFadeOutTime = 0.2f;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
  TObjectPtr<USoundBase> SelectSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
  TObjectPtr<USoundBase> PrepareSound;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
  TObjectPtr<USoundBase> CastSound;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
  TObjectPtr<USoundBase> AbilitySound;

  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	virtual bool GetCanReuse() const;
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	void SetCanReuse(bool _bCanReuse);

  UFUNCTION(BlueprintCallable, Category = "Pestilence|Ability")
	EAbility GetAbilityType() const;

	void SetAbilityType(EAbility _AbilityType);
private:
	TObjectPtr<APestilenceCharacter> Owner;
	bool bCanReuse = false;

	EAbility AbilityType = EAbility::Dummy;

	UPROPERTY()
  TObjectPtr<UAudioComponent> CastSoundComponent;

};
