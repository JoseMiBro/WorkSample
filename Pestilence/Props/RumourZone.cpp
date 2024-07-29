// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Props/RumourZone.h"

#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
//#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

#include "Pestilence/Conversation/ConversationAsset.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/Possession/ArchetypeComponent.h"
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/Core/PestilenceSaveGameSubsystem.h"
#include "Pestilence/Animation/PestilenceCharacter/PestilenceCharacterAnimInstance.h"
#include "Pestilence/Core/PestilenceGameInstance.h"
#include <Pestilence/Core/PestilenceGameMode.h>
#include <Misc/OutputDeviceNull.h>
#include "../UI/PestilenceHUD.h"
#include "../UI/PestilenceConversationUserWidget.h"

// Sets default values
ARumourZone::ARumourZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create triggers volume
	EnterTriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterTriggerVolume"));
	EnterTriggerVolume->SetupAttachment(RootComponent);

	EnterTriggerVolume->SetBoxExtent(FVector(30, 30, 30), true);

	ExitTriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitTriggerVolume"));
	ExitTriggerVolume->SetupAttachment(RootComponent); 
	
	ExitTriggerVolume->SetBoxExtent(FVector(50, 50, 50), true);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	Audio->bAutoActivate = false; // No activar autom�ticamente para evitar que el audio se reproduzca al inicio
	Audio->SetupAttachment(RootComponent); // Asociar el componente al RootComponent de tu Actor
}

// Called when the game starts or when spawned
void ARumourZone::BeginPlay()
{
	Super::BeginPlay();

	APestilenceHUD* HUD = Cast<APestilenceHUD>(UGameplayStatics::GetActorOfClass(this, APestilenceHUD::StaticClass()));
	if (HUD && IsValid(HUD))
  {
    if(HUD->ConversationWidgetInstance)
    {
      HUD->ConversationWidgetInstance->InitializeRumourZone(this);
		}
		else
    {
      HUD->OnWidgetsCreated.AddDynamic(this, &ARumourZone::OnWidgetsCreated);
		}
	}

	EnterTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ARumourZone::OnActorEnterCloseTrigger);
	EnterTriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ARumourZone::OnActorExitCloseTrigger);
	ExitTriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ARumourZone::OnActorExitFarTrigger);

	for (AActor* Character : TalkCharacters)
	{
		APestilenceCharacter* PestilenceCharacter = Cast<APestilenceCharacter>(Character);
		if (PestilenceCharacter) 
		{
			PestilenceCharacter->GetPestilencePossessionComponent()->OnPossessed.AddDynamic(this, &ARumourZone::SetCanTalkFalse);
		}
	}

	//Conversation Internal Set
	if (IsValid(ConversationBase)) 
	{
		InitConversation(ConversationBase);
	}

	if (IsValid(ArchetypeIncorrectConversation))
	{
		InitConversation(ArchetypeIncorrectConversation);
	}

	if (IsValid(AlternativeConversation))
	{
		InitConversation(AlternativeConversation);
	}

	LastConversation = ConversationBase;
	TalkCharactersInTrigger.Empty();

	UGameInstance* GameInstance = GetGameInstance();

	if (GameInstance)
	{
		PestilenceGameInstance = Cast<UPestilenceGameInstance>(GetGameInstance());
		UPestilenceSaveGameSubsystem* GameSubsystem = GetGameInstance()->GetSubsystem<UPestilenceSaveGameSubsystem>();
		if (GameSubsystem)
		{
			GameSubsystem->OnLoad.AddDynamic(this, &ARumourZone::QuickLoadReset);
			GameSubsystem->OnEndLoad.AddDynamic(this, &ARumourZone::IsOvelapp);
		}
	}
}

void ARumourZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  APestilenceHUD* HUD = Cast<APestilenceHUD>(UGameplayStatics::GetActorOfClass(this, APestilenceHUD::StaticClass()));
  if (HUD && IsValid(HUD))
  {
    if (HUD->ConversationWidgetInstance)
    {
      HUD->ConversationWidgetInstance->RemoveRumourZone(this);
    }
    else
    {
      HUD->OnWidgetsCreated.RemoveDynamic(this, &ARumourZone::OnWidgetsCreated);
    }
  }

  EnterTriggerVolume->OnComponentBeginOverlap.RemoveDynamic(this, &ARumourZone::OnActorEnterCloseTrigger);
  EnterTriggerVolume->OnComponentEndOverlap.RemoveDynamic(this, &ARumourZone::OnActorExitCloseTrigger);
  ExitTriggerVolume->OnComponentEndOverlap.RemoveDynamic(this, &ARumourZone::OnActorExitFarTrigger);

  for (AActor* Character : TalkCharacters)
  {
    APestilenceCharacter* PestilenceCharacter = Cast<APestilenceCharacter>(Character);
    if (PestilenceCharacter)
    {
      PestilenceCharacter->GetPestilencePossessionComponent()->OnPossessed.RemoveDynamic(this, &ARumourZone::SetCanTalkFalse);
    }
  }

  PestilenceGameInstance = Cast<UPestilenceGameInstance>(GetGameInstance());
  UPestilenceSaveGameSubsystem* GameSubsystem = GetGameInstance()->GetSubsystem<UPestilenceSaveGameSubsystem>();
  if (GameSubsystem)
  {
    GameSubsystem->OnLoad.RemoveDynamic(this, &ARumourZone::QuickLoadReset);
    GameSubsystem->OnEndLoad.RemoveDynamic(this, &ARumourZone::IsOvelapp);
  }

  Super::EndPlay(EndPlayReason);
}

void ARumourZone::OnWidgetsCreated()
{
  APestilenceHUD* HUD = Cast<APestilenceHUD>(UGameplayStatics::GetActorOfClass(this, APestilenceHUD::StaticClass()));
  if (HUD && IsValid(HUD))
  {
    HUD->OnWidgetsCreated.RemoveDynamic(this, &ARumourZone::OnWidgetsCreated);
		HUD->ConversationWidgetInstance->InitializeRumourZone(this);
  }
}

void ARumourZone::CheckBeginPlay_Implementation()
{
	// Check for overlapping actors
	TArray<AActor*> OverlappingActors;
	EnterTriggerVolume->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() > 0)
	{
		for (AActor* Actor : OverlappingActors)
		{
			SetActorEnter(Actor);
		}
	}
}

void ARumourZone::IsOvelapp()
{
  LineCount = 0;
  OnDisable.Broadcast(this);

	UGameInstance* GameInstance = GetGameInstance();

	CheckBeginPlay();
}

// Called every frame
void ARumourZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimerCheck(DeltaTime);
}

void ARumourZone::TimerCheck(float DeltaTime)
{
	if (bIsPlayerInZone && !PestilenceGameInstance->bLoadGame)
	{
		if (bIsFirstLine)
		{
			return;
		}

		else
		{
			Timer -= DeltaTime;
			if (Timer <= 0 && IsValid(LastConversation))
			{
				if (bIsInDistance) 
				{
					if (!LineCountCheck(LastConversation))
					{
						return;
					}

					if (!BaseConversationCharactersCheck(LastConversation)) 
					{
						return;
					}

					if (SpecialCheck(LastConversation))
					{
						Conversation(LastConversation);
						return;
					}
				}
				ConfirmationConversation(LastConversation);
			}
		}
	}
}

void ARumourZone::InitConversation(UConversationAsset* _ConversationAsset)
{
	bool auxImportant = _ConversationAsset->IsConversationImportant();
	_ConversationAsset->bOnFinishConversation = false;
	if (auxImportant) 
	{
		bIsTriggerImportant = auxImportant;
		ChangeImportantZone(bIsTriggerImportant);
	}
}

void ARumourZone::PlayAnimation(UConversationAsset* _ConversationAsset)
{
  StopAnimation(_ConversationAsset);
	if (IsValid(_ConversationAsset) && _ConversationAsset->Lines.IsValidIndex(LineCount)) 
	{
		if (TalkCharacters.IsValidIndex(_ConversationAsset->Lines[LineCount].IDTalkCharacter)) 
    {
      APestilenceCharacter* Character = Cast<APestilenceCharacter>(TalkCharacters[_ConversationAsset->Lines[LineCount].IDTalkCharacter]);
			if (!Character) return;
      Character->OnTalkRumour.Broadcast(true);
			if (IsValid(_ConversationAsset->Lines[LineCount].TalkAnimation)) 
			{
					if (Character->GetMesh() && Character->GetMesh()->GetAnimInstance())
					{
						TObjectPtr<UPestilenceCharacterAnimInstance> AnimInstance = Cast<UPestilenceCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance());
						if (AnimInstance)
						{
							//PlayAnim
							AnimInstance->RumourAnimation = _ConversationAsset->Lines[LineCount].TalkAnimation;
							AnimInstance->bIsInRumour = true;
						}
					}
			}
		}
	}
}

void ARumourZone::StopAnimation(UConversationAsset* _ConversationAsset)
{
	int auxCount = -1;

	if (IsValid(_ConversationAsset) && _ConversationAsset->Lines.IsValidIndex(LineCount + auxCount))
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
					if (_ConversationAsset->Lines[LineCount + auxCount].RestrictedGenders != EGender::Other)
					{
						EGender Gender = ArchetypeComponent->GetPestilenceGender();

						if (LineCount + auxCount < _ConversationAsset->Lines.Num())
						{
							if (Gender == EGender::Male && _ConversationAsset->Lines[LineCount + auxCount].RestrictedGenders == EGender::Female)
							{
								--auxCount;
							}

							if (Gender == EGender::Female && _ConversationAsset->Lines[LineCount + auxCount].RestrictedGenders == EGender::Male)
							{
								--auxCount;
							}
						}
					}
				}
			}

			if (IsValid(_ConversationAsset) && _ConversationAsset->Lines.IsValidIndex(LineCount + auxCount))
			{
				if (TalkCharacters.IsValidIndex(_ConversationAsset->Lines[LineCount + auxCount].IDTalkCharacter))
				{
					APestilenceCharacter* Character = Cast<APestilenceCharacter>(TalkCharacters[_ConversationAsset->Lines[LineCount + auxCount].IDTalkCharacter]);
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
	}
}

bool ARumourZone::ArchetypeAndGenderCheck(UConversationAsset* _ConversationAsset, UConversationAsset* _ArchetypeIncorrectConversation)
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
				EArchetype Archetype = ArchetypeComponent->GetPestilenceArchetype();
				if (_ConversationAsset->RestrictedArchetypes.Contains(Archetype))
				{
					if (IsValid(ArchetypeIncorrectConversation) && _ConversationAsset != _ArchetypeIncorrectConversation)
					{
						ConfirmationConversation(ArchetypeIncorrectConversation);
					}
					return false;
				}

				if (_ConversationAsset->Lines[LineCount].RestrictedGenders != EGender::Other)
				{
					EGender Gender = ArchetypeComponent->GetPestilenceGender();

					if (LineCount + 1 < _ConversationAsset->Lines.Num()) 
					{
						if (Gender == EGender::Male && _ConversationAsset->Lines[LineCount].RestrictedGenders == EGender::Female)
						{
							++LineCount;
						}

						if (Gender == EGender::Female && _ConversationAsset->Lines[LineCount].RestrictedGenders == EGender::Male)
						{
							++LineCount;
						}

						if (LineCount >= _ConversationAsset->Lines.Num()) 
						{
							ConfirmationConversation(_ConversationAsset);
						}
					}
				}
			}
		}
	}
	return true;
}

bool ARumourZone::IsSameConversation(UConversationAsset* _ConversationAsset)
{
	if (LastConversation) 
	{
		if (LastConversation != _ConversationAsset)
		{
			LineCount = 0;

			LastConversation = _ConversationAsset;
			ConfirmationConversation(_ConversationAsset);

			return false;
		}
	}
	return true;
}

bool ARumourZone::LineCountCheck(UConversationAsset* _ConversationAsset)
{
	if (IsValid(_ConversationAsset) && bIsPlayerInZone)
	{
		if (LineCount >= _ConversationAsset->Lines.Num())
		{
			StopAnimation(_ConversationAsset);
			LineCount = 0; 
			
			if (IsValid(_ConversationAsset->Lines[LineCount].SoundLine))
			{
				Timer = _ConversationAsset->Lines[LineCount].SoundLine->GetDuration() + _ConversationAsset->Lines[LineCount].TextOffsetDuration;
			}

			else
			{
				Timer = _ConversationAsset->Lines[LineCount].TextOffsetDuration;
			}

			_ConversationAsset->bOnFinishConversation = true;

      OnDisable.Broadcast(this);

			return false;
		}

		if (LineCount < 0)
		{
			LineCount = 0;
			return false;
		}
		return true;
	}
	return false;
}

bool ARumourZone::NotLoopAndChangeCheck(UConversationAsset* _ConversationAsset, UConversationAsset* _AlternativeAsset)
{
	if (_ConversationAsset->bOnFinishConversation && !_ConversationAsset->CanLoop)
	{
		if (IsValid(_AlternativeAsset) && _ConversationAsset != _AlternativeAsset)
		{
			ConfirmationConversation(_AlternativeAsset);
		}
		else 
    {
      OnDisable.Broadcast(this);
		}
		return false;
	}
	return true;
}

bool ARumourZone::ConversationCharactersCheck(UConversationAsset* _ConversationAsset)
{
	if (!BaseConversationCharactersCheck(_ConversationAsset)) 
	{
		return false;
	}

	for (int i = 0; i < TalkCharacters.Num(); ++i)
	{
		if (TalkCharacters.IsValidIndex(i) && !TalkCharactersInTrigger.Contains(TalkCharacters[i]))
    {
      OnDisable.Broadcast(this);
			return false;
		}
		return true;
	}

	return false;
}

bool ARumourZone::BaseConversationCharactersCheck(UConversationAsset* _ConversationAsset)
{
	if (IsValid(_ConversationAsset) && _ConversationAsset->Lines.IsValidIndex(LineCount)
		&& TalkCharacters.IsValidIndex(_ConversationAsset->Lines[LineCount].IDTalkCharacter)
		&& IsValid(TalkCharacters[_ConversationAsset->Lines[LineCount].IDTalkCharacter]))
	{
		for (int i = 0; i < TalkCharacters.Num(); ++i)
		{
			if (TalkCharacters.IsValidIndex(i) && IsValid(TalkCharacters[i]))
			{
				UPestilencePossessionComponent* pPossessionComponent = TalkCharacters[i]->GetPestilencePossessionComponent();
				if (IsValid(pPossessionComponent))
				{
					if (pPossessionComponent->IsPossessed())
					{
						if (OnSetOwner.IsBound())
            {
              OnDisable.Broadcast(this);
						}
						return false;
					}
				}
			}

			if (TalkCharacters.IsValidIndex(i) && IsValid(TalkCharacters[i]) && IsValid(TalkCharacters[i]->GetController()))
			{
				APestilenceAIController* pController = Cast<APestilenceAIController>(TalkCharacters[i]->GetController());
				if (pController)
				{
					if (pController->GetCurrentState() == EPestilenceStates::Infected || pController->GetCurrentState() == EPestilenceStates::Stun)
          {
            OnDisable.Broadcast(this);
						return false;
					}
				}
			}
		}
		if (!bIsPlayerInZone)
    {
      OnDisable.Broadcast(this);

			NotLoopAndChangeCheck(_ConversationAsset, AlternativeConversation);

			return false;
		}
		return true;
	}
	return false;
}

bool ARumourZone::BaseCheck(UConversationAsset* _ConversationAsset)
{
	if (!LineCountCheck(_ConversationAsset))
	{
		return false;
	}

	if (!ConversationCharactersCheck(_ConversationAsset))
	{
		return false;
	}
	
	return true;
}

bool ARumourZone::SpecialCheck_Implementation(UConversationAsset* _ConversationAsset)
{
	if (!ArchetypeAndGenderCheck(_ConversationAsset, ArchetypeIncorrectConversation))
	{
		return false;
	}

	if (!NotLoopAndChangeCheck(_ConversationAsset, AlternativeConversation))
	{
		return false;
	}
	return true;
}

bool ARumourZone::IsImportantCheck(UConversationAsset* _ConversationAsset, int _LineCount)
{
	if (_ConversationAsset->Lines[_LineCount].IsLineImportant)
	{
		if (_ConversationAsset->DialogueImportantCount > 0)
		{
			--_ConversationAsset->DialogueImportantCount;
			OnImportantText.Broadcast(_ConversationAsset->Lines[_LineCount].IDDiaryText);
		}
		else
		{
			_ConversationAsset->IsDialogueImportant = false;
			if (IsAllImportantCheck()) 
			{
				bIsTriggerImportant = true;
				ChangeImportantZone(bIsTriggerImportant);
			}
		}
	}
	return _ConversationAsset->IsDialogueImportant;
}

bool ARumourZone::ObjectiveCheck(UConversationAsset* _ConversationAsset)
{
	if (_ConversationAsset->Lines[LineCount].ChangesObjective)
	{
		APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
		UpdateObjective(_ConversationAsset->Lines[LineCount].ObjectiveIndex);
	}
	return _ConversationAsset->IsDialogueImportant;
}

void ARumourZone::UpdateObjective_Implementation(int Index) 
{

}

bool ARumourZone::IsAllImportantCheck()
{
	bool auxConfirmation = false;
	if (IsValid(ConversationBase))
	{
		if (ConversationBase.Get()->IsDialogueImportant) 
		{
			auxConfirmation = true;
		}
	}

	if (IsValid(ArchetypeIncorrectConversation))
	{
		if(ArchetypeIncorrectConversation.Get()->IsDialogueImportant)
		{
			auxConfirmation = true;
		}
	}

	if (IsValid(AlternativeConversation))
	{
		if (ArchetypeIncorrectConversation.Get()->IsDialogueImportant)
		{
			auxConfirmation = true;
		}
	}
	return auxConfirmation;
}

void ARumourZone::ConfirmationConversation(UConversationAsset* _ConversationAsset)
{
	if (bIsPlayerInZone) 
	{
		if (BaseCheck(_ConversationAsset))
		{
			if (SpecialCheck(_ConversationAsset))
			{
				Conversation(_ConversationAsset);
				return;
			}
		}
	}

	bIsConversationStarted = false;
	OnConversationEnd.Broadcast();
}

void ARumourZone::Conversation(UConversationAsset* _ConversationAsset)
{
	if (!IsSameConversation(_ConversationAsset))
	{
		return;
	}

	if (!bIsConversationStarted)
	{
		bIsConversationStarted = true;
		OnConversationStart.Broadcast();
	}

	//Conexion with Widget
	if (UGameplayStatics::AreSubtitlesEnabled())
	{
		if (OnSetOwner.IsBound())
		{
      //OnSetOwner.Broadcast(this);
      OnRange.Broadcast(bShouldShowConversation, this);
			OnConversationText.Broadcast(_ConversationAsset->Lines[LineCount].Text, _ConversationAsset->Lines[LineCount].IsLineImportant,
				TalkCharacters[_ConversationAsset->Lines[LineCount].IDTalkCharacter], IsImportantCheck(_ConversationAsset, LineCount), this);
		}
	}

	else
  {
    OnDisable.Broadcast(this);
	}
	PlayAnimation(_ConversationAsset);


	if (IsValid(_ConversationAsset->Lines[LineCount].SoundLine)) 
	{
		if (IsValid(TalkCharacters[_ConversationAsset->Lines[LineCount].IDTalkCharacter])) 
		{
			// Establece la ubicaci�n del componente de audio
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

    OnDisable.Broadcast(this);
	}
	++LineCount;

	LastConversation = _ConversationAsset;
}

void ARumourZone::OnActorEnterCloseTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetActorEnter(OtherActor);
}

void ARumourZone::SetActorEnter(AActor* OtherActor)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		APestilenceCharacter* PestilenceCharacter = Cast<APestilenceCharacter>(OtherActor);
		if (PestilenceCharacter)
		{
			if (PestilenceCharacter->GetController() != PlayerController)
			{
				if (TalkCharacters.Contains(PestilenceCharacter))
				{
					if (!TalkCharactersInTrigger.Contains(PestilenceCharacter))
					{
						TalkCharactersInTrigger.Add(PestilenceCharacter);

						//Is All PestillenceCharacter? 
						if (ConversationCharactersCheck(LastConversation))
						{
							ConfirmationConversation(LastConversation);
						}
					}
				}
			}

			else
			{
				if (IsValid(LastConversation) && !LastConversation->bOnFinishConversation)
				{
					if (IsValid(TalkCharacters[0]))
					{
            for (int i = 0; i < TalkCharacters.Num(); ++i)
            {
              //Is All PestillenceCharacter?
              if (!TalkCharactersInTrigger.Contains(TalkCharacters[i]) || TalkCharacters[i]->GetPestilencePossessionComponent()->IsPossessed())
              {
                return;
              }
            }
            bShouldShowConversation = true;
						if (!bIsPlayerInZone) 
						{
              bIsPlayerInZone = true;
              bIsInDistance = false;
							if (bCanTalk) 
              {
                OnSetOwner.Broadcast(this);
								ConfirmationConversation(LastConversation);
							}
						}
						else
            {
              if (UGameplayStatics::AreSubtitlesEnabled())
              {
                if (OnSetOwner.IsBound())
                {
                  OnSetOwner.Broadcast(this);
                  OnRange.Broadcast(true, this);
									int AuxLineCount = LineCount <= 0 ? 0 : LineCount - 1;
                  OnConversationText.Broadcast(LastConversation->Lines[AuxLineCount].Text, LastConversation->Lines[AuxLineCount].IsLineImportant,
                    TalkCharacters[LastConversation->Lines[AuxLineCount].IDTalkCharacter], IsImportantCheck(LastConversation, AuxLineCount), this);
                }
              }
						}
					}
				}
			}
		}
	}
}

void ARumourZone::OnActorExitCloseTrigger(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
  if (Player != OtherActor) return;

  bShouldShowConversation = false;
  OnRange.Broadcast(false, this);
}

void ARumourZone::OnActorExitFarTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		TObjectPtr<APestilenceCharacter> PestilenceCharacter = Cast<APestilenceCharacter>(OtherActor);
		if (PestilenceCharacter) 
		{
			if (PestilenceCharacter->GetController() != PlayerController)
			{
				if (TalkCharactersInTrigger.Contains(PestilenceCharacter))
				{
					TalkCharactersInTrigger.Remove(PestilenceCharacter);
					StopAnimation(LastConversation);
					//Stop Behaviour
				}
			}
			else
			{
				//DesactivateWidget
				if (bIsPlayerInZone && IsValid(LastConversation)) 
				{
					bIsInDistance = false;
					bIsPlayerInZone = false;

					if (bCanTalk) 
					{
						bIsConversationStarted = false;
						OnConversationEnd.Broadcast();
						StopAnimation(LastConversation);

						--LineCount;
						LineCount = LineCount < 0 ? 0 : LineCount;

						BaseCheck(LastConversation);

						Timer = 0.f;

						bIsFirstLine = true;

						if (Audio && Audio->IsPlaying())
						{
							Audio->Stop();
						}

						if (OnSetOwner.IsBound())
						{
							OnSetOwner.Broadcast(this);
							if (LastConversation->bOnFinishConversation)
              {
                OnDisable.Broadcast(this);
							}
							else
							{
								OnRange.Broadcast(false, this);
							}


							for (int i = 0; i < TalkCharacters.Num(); ++i)
							{
								if (TalkCharacters.IsValidIndex(i) && IsValid(TalkCharacters[i]))
								{
									UPestilencePossessionComponent* pPossessionComponent = TalkCharacters[i]->GetPestilencePossessionComponent();
									if (IsValid(pPossessionComponent))
									{
										if (pPossessionComponent->IsPossessed())
                    {
                      OnDisable.Broadcast(this);
											return;
										}
									}
								}

								if (TalkCharacters.IsValidIndex(i) && IsValid(TalkCharacters[i]) && IsValid(TalkCharacters[i]->GetController()))
								{
									APestilenceAIController* pController = Cast<APestilenceAIController>(TalkCharacters[i]->GetController());
									if (pController)
									{
										if (pController->GetCurrentState() == EPestilenceStates::Infected || pController->GetCurrentState() == EPestilenceStates::Stun)
                    {
                      OnDisable.Broadcast(this);
											return;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void ARumourZone::QuickLoadReset()
{
	bIsPlayerInZone = false;
	bIsConversationStarted = false;
	OnConversationEnd.Broadcast();
	StopAnimation(LastConversation);

	LineCount = 0;
	Timer = 0.f;
	bIsFirstLine = true;
	
	BaseCheck(LastConversation);

	if (Audio && Audio->IsPlaying())
	{
		Audio->Stop();
	}

	if (OnSetOwner.IsBound())
	{
		OnSetOwner.Broadcast(this);
		if (IsValid(LastConversation)) 
		{
			if (LastConversation->bOnFinishConversation)
      {
        OnDisable.Broadcast(this);
			}
			else
			{
				OnRange.Broadcast(false, this);
			}
		}
		else 
		{
      OnDisable.Broadcast(this);
		}
	}
}

void ARumourZone::ActivateRumourDistance()
{
	bIsInDistance = true;
	bIsPlayerInZone = true; 
	bShouldShowConversation = true; 
	OnSetOwner.Broadcast(this);
	Conversation(LastConversation);
}

void ARumourZone::DisableRumourDistance()
{
	if (bIsPlayerInZone && IsValid(LastConversation))
	{
		bIsPlayerInZone = false;
		bIsConversationStarted = false;
		bIsInDistance = false;
		OnConversationEnd.Broadcast();
		StopAnimation(LastConversation);

    --LineCount;
    LineCount = LineCount < 0 ? 0 : LineCount;

		BaseCheck(LastConversation);

		Timer = 0.f;

		bIsFirstLine = true;

		if (Audio && Audio->IsPlaying())
		{
			Audio->Stop();
		}

		if (LastConversation->bOnFinishConversation)
    {
      OnDisable.Broadcast(this);
		}
		else
		{
			OnRange.Broadcast(false, this);
		}
	}
}

void ARumourZone::SetCanTalkFalse()
{
  bCanTalk = false;
  DisableRumourDistance();
	APestilenceCharacter* Player = Cast<APestilenceCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player && TalkCharactersInTrigger.Contains(Player))
		TalkCharactersInTrigger.Remove(Player);
}

void ARumourZone::SetCanTalkTrue()
{
	bCanTalk = true;
}
