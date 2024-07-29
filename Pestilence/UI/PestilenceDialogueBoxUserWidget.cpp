// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceDialogueBoxUserWidget.h"

#include "Pestilence/Characters/PestilenceDialogueCharacter.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPestilenceDialogueBoxUserWidget::NativeConstruct()
{
  Super::NativeConstruct();
}

bool UPestilenceDialogueBoxUserWidget::Initialize()
{
	bool parentSuccess = Super::Initialize();

	if (!parentSuccess)
	{
		return false;
	}

	if (GetWorld())
	{
		TArray<AActor*> NPCs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APestilenceDialogueCharacter::StaticClass(), NPCs);

		for (AActor* Actor : NPCs)
		{
			APestilenceDialogueCharacter* NPCDialogue = Cast<APestilenceDialogueCharacter>(Actor);

			if (IsValid(NPCDialogue))
			{
				NPCDialogue->OnDialogueDelegate.AddDynamic(this, &UPestilenceDialogueBoxUserWidget::SetDialogueText);
			}
		}
	}
  return true;
}

void UPestilenceDialogueBoxUserWidget::SetDialogueText(FString _SpeakerName, FString _Dialogue)
{
	SpeakerName->SetText(FText::FromString(_SpeakerName));
	DialogueText->SetText(FText::FromString(_Dialogue));
}
