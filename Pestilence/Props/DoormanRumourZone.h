// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RumourZone.h"
#include "DoormanRumourZone.generated.h"

/**
 * 
 */

class APestilenceHUD;

UCLASS()
class PESTILENCE_API ADoormanRumourZone : public ARumourZone
{
	GENERATED_BODY()

	bool bCanPlay = true; 

protected:
	virtual void BeginPlay() override;

public:
	APestilenceHUD* PestilenceHUD;
	void CorrectConversation(UConversationAsset* _ConversationAsset);
	void InCorrectConversation(UConversationAsset* _ConversationAsset);
	virtual void Tick(float DeltaTime) override;

	public:
	virtual void TimerCheck(float DeltaTime) override;

		UPROPERTY(EditAnywhere,Category="Pestilence Conversations")
			TObjectPtr<UConversationAsset> CanEnterConversationAsset;
		UPROPERTY(EditAnywhere,Category="Pestilence Conversations")
			TObjectPtr<UConversationAsset> CanNotEnterConversationAsset;
		virtual void Conversation(UConversationAsset* _ConversationAsset) override;
};
