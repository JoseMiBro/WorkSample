// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceDialogueBoxUserWidget.generated.h"

class UTextBlock;
class USizeBox;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceDialogueBoxUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		USizeBox* DialogueBox;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SpeakerName;
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DialogueText;

protected:
	virtual void NativeConstruct() override;

public:
	virtual bool Initialize() override;

private:
	UFUNCTION()
		void SetDialogueText(FString _SpeakerName, FString _Dialogue);

};
