// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceConversationUserWidget.generated.h"

class APestilenceCharacter;
class ARumourZone;


UCLASS()
class PESTILENCE_API UPestilenceConversationUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Conversation")
	TObjectPtr<ARumourZone> Rumour;

public:
	virtual bool Initialize() override;

public:
	void InitializeRumourZone(ARumourZone* _RumourZone);
	void RemoveRumourZone(ARumourZone* _RumourZone);

	UFUNCTION(BlueprintImplementableEvent)
		void OnTextModified(const FText& ConversationText, bool IsImportantLine, APestilenceCharacter* TalkCharacter, bool IsDialogueImportant, ARumourZone* Owner);

		UFUNCTION(BlueprintImplementableEvent)
		void SetOwner(ARumourZone* Owner);

	UFUNCTION(BlueprintImplementableEvent)
		void OnRangeModified(bool IsInTextRange, ARumourZone* Owner);

	UFUNCTION(BlueprintImplementableEvent)
		void DisableAll(ARumourZone* Owner);
};
