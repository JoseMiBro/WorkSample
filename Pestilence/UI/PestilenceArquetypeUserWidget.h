// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceArquetypeUserWidget.generated.h"

class APestilenceCharacter;
class UArchetypeComponent;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceArquetypeUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  virtual bool Initialize() override;
  
	UFUNCTION(BlueprintImplementableEvent)
	void OnArchetypeModified(UArchetypeComponent* _ArchetypeComponent);
	UFUNCTION(BlueprintImplementableEvent)
	void OnZoneChanged(bool _bIsInCorrectZone, bool _bIsInForbiddenZone);

  UFUNCTION()
  void OnEndPossess();

private:
  void BindPlayer();
  TObjectPtr<APestilenceCharacter> Player;
};
