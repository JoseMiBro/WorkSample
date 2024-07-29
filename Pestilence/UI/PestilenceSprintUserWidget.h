// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceSprintUserWidget.generated.h"

class UImage;
class APestilenceCharacter;

UCLASS()
class PESTILENCE_API UPestilenceSprintUserWidget : public UUserWidget
{
	GENERATED_BODY()

//public:
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		UImage* SprintImage;

public:
  virtual bool Initialize() override;

  UFUNCTION(BlueprintImplementableEvent)
  void OnSprintModified(bool IsSprint);

  UFUNCTION()
  void OnEndPossess();

private:
  void BindPlayer();
  TObjectPtr<APestilenceCharacter> Player;
};
