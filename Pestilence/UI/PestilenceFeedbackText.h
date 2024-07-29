// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceFeedbackText.generated.h"

class UTextBlock;
class USizeBox;
class APestilenceCharacter;
class UIActorVisionComponent;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceFeedbackText : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* InteractText;
	
	UPROPERTY(meta = (BindWidget))
		USizeBox* InteractSizeBox;

	UPROPERTY()
		APestilenceCharacter* pCharacter;

	UPROPERTY()
		UIActorVisionComponent* pInteractComponent;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void SetInteractText(FString FeedbackText);

	UFUNCTION()
		void SetExitInteractText();

};
