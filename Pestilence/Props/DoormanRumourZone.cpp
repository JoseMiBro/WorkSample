// Fill out your copyright notice in the Description page of Project Settings.


#include "DoormanRumourZone.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Conversation/ConversationAsset.h"
#include "Pestilence/Core/PestilenceGameInstance.h"
#include "Pestilence/Components/Possession/ArchetypeComponent.h"
#include "Pestilence/UI/PestilenceHUD.h"
#include "Pestilence/UI/PestilenceConversationUserWidget.h"
#include "Pestilence/Animation/PestilenceCharacter/PestilenceCharacterAnimInstance.h"
#include "Pestilence/Conversation/ConversationAsset.h"


void ADoormanRumourZone::BeginPlay()
{
	Super::BeginPlay();
	EnterTriggerVolume->OnComponentBeginOverlap.RemoveDynamic(this, &ARumourZone::OnActorEnterCloseTrigger);
	EnterTriggerVolume->OnComponentEndOverlap.RemoveDynamic(this, &ARumourZone::OnActorExitCloseTrigger);
	ExitTriggerVolume->OnComponentEndOverlap.RemoveDynamic(this, &ARumourZone::OnActorExitFarTrigger); 

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (GetWorld())
	{
		if (PlayerController && PlayerController->GetHUD())
		{
			PestilenceHUD = Cast<APestilenceHUD>(PlayerController->GetHUD());
		}
	}
}

void ADoormanRumourZone::CorrectConversation(UConversationAsset* _ConversationAsset)
{
	if (bCanPlay)
	{
		bCanPlay = false;
		if (IsValid(_ConversationAsset) && _ConversationAsset->Lines.IsValidIndex(LineCount))
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PlayerController)
			{
				APestilenceCharacter* PestilenceCharacter = Cast<APestilenceCharacter>(PlayerController->GetPawn());
				if (PestilenceCharacter)
				{
					UArchetypeComponent* ArchetypeComponent = PestilenceCharacter->GetArchetypeComponent();
					if (ArchetypeComponent)
					{
						if (_ConversationAsset->Lines[LineCount].RestrictedGenders != EGender::Other)
						{
							EGender Gender = ArchetypeComponent->GetPestilenceGender();

              LineCount = 0;
							bool bFound = false;
              while (LineCount < _ConversationAsset->Lines.Num() && !bFound)
              {
								bFound = _ConversationAsset->Lines[LineCount].RestrictedGenders == Gender;
								++LineCount;
              }
							--LineCount;
							if (!bFound) return;
						}
						Conversation(_ConversationAsset);
					}
				}
			}
		}
	}
}

void ADoormanRumourZone::InCorrectConversation(UConversationAsset* _ConversationAsset)
{
	if (bCanPlay)
	{
		bCanPlay = false;
		LineCount = FMath::RandRange(0, _ConversationAsset->Lines.Num() - 1);
		Conversation(_ConversationAsset);
	}
}

void ADoormanRumourZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoormanRumourZone::TimerCheck(float DeltaTime)
{
	if (!PestilenceGameInstance->bLoadGame && bIsPlayerInZone)
	{
		if (bIsFirstLine)
		{
			return;
		}

		else
		{
			Timer -= DeltaTime;
			if (Timer <= 0)
			{
				PestilenceHUD->ConversationWidgetInstance->DisableAll(this);
				Timer = 0.f;
				bIsFirstLine = true;
				bIsPlayerInZone = false;

				if (LastConversation)
				{
					for (int i = 0; i < TalkCharacters.Num(); ++i)
					{
						if (TalkCharacters.IsValidIndex(i))
						{
							APestilenceCharacter* Character = Cast<APestilenceCharacter>(TalkCharacters[i]);
							if (Character)
							{
								Character->OnTalkRumour.Broadcast(false);
								if (Character->GetMesh() && Character->GetMesh()->GetAnimInstance())
								{
									TObjectPtr<UPestilenceCharacterAnimInstance> AnimInstance = Cast<UPestilenceCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance());
									if (AnimInstance)
									{
										//StopAnim
										AnimInstance->bIsInRumour = false;
									}
								}
							}
						}
					}
				}

				OnConversationEnd.Broadcast();
				bCanPlay = true;
			}
		}
	}
}

void ADoormanRumourZone::Conversation(UConversationAsset* _ConversationAsset)
{
	//if (bCanPlay) 
	{
    bCanPlay = false;
		if (!bIsConversationStarted)
		{
			bIsConversationStarted = true;
			OnConversationStart.Broadcast();
		}

		//Conexion with Widget
		if (UGameplayStatics::AreSubtitlesEnabled())
    {
      OnSetOwner.Broadcast(this);
      OnRange.Broadcast(true, this);
			PestilenceHUD->ConversationWidgetInstance->OnTextModified(_ConversationAsset->Lines[LineCount].Text, _ConversationAsset->Lines[LineCount].IsLineImportant,
				TalkCharacters[_ConversationAsset->Lines[LineCount].IDTalkCharacter], IsImportantCheck(_ConversationAsset, LineCount), this);
		}

		else
		{
			PestilenceHUD->ConversationWidgetInstance->DisableAll(this);
		}

		PlayAnimation(_ConversationAsset);

		if (IsValid(_ConversationAsset->Lines[LineCount].SoundLine))
		{
			if (IsValid(TalkCharacters[_ConversationAsset->Lines[LineCount].IDTalkCharacter]))
			{
				// Establece la ubicaci?n del componente de audio
				Audio->SetWorldLocation(TalkCharacters[_ConversationAsset->Lines[LineCount].IDTalkCharacter].Get()->GetActorLocation());
				Audio->SetSound(_ConversationAsset->Lines[LineCount].SoundLine);

				// Reproduce el audio
				Audio->Play();

				//Cambia el objetivo si ChangesObjective es true
				ObjectiveCheck(_ConversationAsset);

				//Llama a una función del BP si LaunchesFunct es true
				if (_ConversationAsset->Lines[LineCount].LaunchesFunct)
				{
					const TCHAR* name = *_ConversationAsset->Lines[LineCount].FunctionName;
					FOutputDeviceNull OutputDeviceNull;
					this->CallFunctionByNameWithArguments(name, OutputDeviceNull, nullptr, true);
				}

				Timer = _ConversationAsset->Lines[LineCount].SoundLine->GetDuration() + _ConversationAsset->Lines[LineCount].TextOffsetDuration;
				bIsFirstLine = false;
			}
		}
		else
		{
			Timer = _ConversationAsset->Lines[LineCount].TextOffsetDuration;
			bIsFirstLine = false;

			if (OnSetOwner.IsBound())
			{
				PestilenceHUD->ConversationWidgetInstance->DisableAll(this);
			}
		}

		LastConversation = _ConversationAsset;
	}
}
