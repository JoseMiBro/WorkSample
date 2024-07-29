// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceCrouchUserWidget.generated.h"

class APestilenceCharacter;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceCrouchUserWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:
  virtual bool Initialize() override;

  UFUNCTION(BlueprintImplementableEvent)
  void OnCrouchModified(bool IsCrouch);

  UFUNCTION()
  void OnEndPossess();

private:
  void BindPlayer();
  TObjectPtr<APestilenceCharacter> Player;
};
