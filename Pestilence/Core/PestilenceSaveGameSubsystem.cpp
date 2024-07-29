#include "Pestilence/Core/PestilenceSaveGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include <EngineUtils.h>
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Pestilence/UI/PestilenceDiaryUserWidget.h"
#include "Pestilence/Components/Interaction/PickableComponent.h"
#include "Pestilence/Core/CheckPoint.h"
#include "../Components/Abilities/PestilenceAbilityComponent.h"
#include "PestilenceGameMode.h"
#include "../Components/Movement/PestilenceMovementComponent.h"
#include "Engine/LevelStreaming.h"
#include "Engine/Level.h"
#include "Pestilence/InteractiveActors/IActors/IA_SwingDoor.h"
#include "Pestilence/Components/Possession/PossessedState.h"
#include "Pestilence/Components/Possession/TransitionState.h"
#include "Pestilence/UI/PestilenceHUD.h"
#include "Pestilence/Core/PestilenceGameInstance.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Pestilence/Props/PostProcessBlendZone.h"
#include "../Misc/PestilenceCollisionChannels.h"
#include "Engine/LevelStreamingVolume.h"
#include "Camera/CameraComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "../Components/Interaction/InteractorComponent.h"
#include "../Characters/NPCManager.h"
#include "../Components/Possession/ArchetypeComponent.h"
#include "../Components/MixAndMatch/MixAndMatchComponent.h"



void UPestilenceSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UPestilenceSaveGameSubsystem::Deinitialize()
{
}

void UPestilenceSaveGameSubsystem::SetSlotName(FString& NewSlotName)
{
    if (NewSlotName.Len() == 0) 
    {
        return;
    }

    CurrentSlotName = NewSlotName;
}

void UPestilenceSaveGameSubsystem::QuickSave()
{
    OnSave.Broadcast();

    USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0);
    SaveGameObject = Cast<UPestilenceSaveGame>(LoadedGame);

    if (!SaveGameObject)
    {
        SaveGameObject = Cast<UPestilenceSaveGame>(UGameplayStatics::CreateSaveGameObject(UPestilenceSaveGame::StaticClass()));
    }


    SaveGameObject->SavedCheckPoints.Empty();
    TObjectPtr<APestilenceCharacter> CurrentPestPlayer = Cast<APestilenceCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
    CurrentPestPlayer->bIsDead = true;
    //Iterar world para cojer PestChars
    for (FActorIterator It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        // Only interested in our 'Pestilence actors'
        if (Actor->GetClass()->IsChildOf(APestilenceCharacter::StaticClass()))
        {
            APestilenceCharacter* PestActor = Cast<APestilenceCharacter>(Actor);
            AAIController* AIController = PestActor->GetControllerAI();
            FActorSaveData* ExistingActorData = nullptr;
            for (FActorSaveData& ActorData : SaveGameObject->SavedActors)
            {
                if (ActorData.CharacterID == PestActor->ID)
                {
                    ExistingActorData = &ActorData;
                    break;
                }
            }
            if (ExistingActorData)  // If the actor exists in the array, update its information
            {
                ExistingActorData->Transform = PestActor->GetActorTransform();
                ExistingActorData->bIsDead = PestActor->bIsDead;
                if (AIController)
                {
                    ExistingActorData->SavedTargetPointIndex = AIController->GetBlackboardComponent()->GetValueAsInt(Keys::TargetPointIndex);
                }
            }
            else // Actor doesn't exist, create and add its information
            {
                FActorSaveData ActorData;
                ActorData.CharacterID = PestActor->ID;
                ActorData.Transform = PestActor->GetActorTransform();
                ActorData.bIsDead = PestActor->bIsDead;
                if (AIController)
                {
                    ActorData.SavedTargetPointIndex = AIController->GetBlackboardComponent()->GetValueAsInt(Keys::TargetPointIndex);
                }
                SaveGameObject->SavedActors.Add(ActorData);
            }
        }
        else if (Actor->GetClass()->IsChildOf(ACheckPoint::StaticClass()))
        {
            ACheckPoint* CurrentCheckPoint = Cast<ACheckPoint>(Actor);
            FCheckPointSaveData CheckPointData;

            CheckPointData.CheckPointID = CurrentCheckPoint->ID;
            CheckPointData.bIsConsumed = CurrentCheckPoint->GetCheckPointState();

            SaveGameObject->SavedCheckPoints.Add(CheckPointData);
        }
        else if (Actor->GetClass()->IsChildOf(AIA_SwingDoor::StaticClass()))
        {
            AIA_SwingDoor* SwingDoorActor = Cast<AIA_SwingDoor>(Actor);
            FDoorSaveData* ExistingDoorData = nullptr;
            for (FDoorSaveData& ActorData : SaveGameObject->SavedSwingDoors)
            {
                if (ActorData.DoorID == SwingDoorActor->ID)
                {
                    ExistingDoorData = &ActorData;
                    break;
                }
            }

            if (ExistingDoorData)  // If the actor exists in the array, update its information
            {
                ExistingDoorData->bIsLocked = SwingDoorActor->bLocked;
                ExistingDoorData->bIsOpen = SwingDoorActor->bOpen;
            }
            else // Actor doesn't exist, create and add its information
            {
                FDoorSaveData SwingDoorData;

                SwingDoorData.DoorID = SwingDoorActor->ID;
                SwingDoorData.bIsLocked = SwingDoorActor->bLocked;
                SwingDoorData.bIsOpen = SwingDoorActor->bOpen;

                SaveGameObject->SavedSwingDoors.Add(SwingDoorData);
            }
        }
        else
        {
            continue;
        }
    }

    for (FDoorSaveData& SwingDoor : SavedSwingDoors)
    {
      FDoorSaveData* ExistingDoorData = nullptr;
      for (FDoorSaveData& ActorData : SaveGameObject->SavedSwingDoors)
      {
        if (ActorData.DoorID == SwingDoor.DoorID)
        {
          ExistingDoorData = &ActorData;
          break;
        }
      }

      if (ExistingDoorData)  // If the actor exists in the array, update its information
      {
        ExistingDoorData->bIsLocked = SwingDoor.bIsLocked;
        ExistingDoorData->bIsOpen = SwingDoor.bIsOpen;
      }
      else // Actor doesn't exist, create and add its information
      {
        SaveGameObject->SavedSwingDoors.Add(SwingDoor);
      }
    }

    
    //Integrity, player loc
    SaveGameObject->SavedIntegrity = CurrentPestPlayer->GetPestilencePossessionComponent()->GetCurrentIntegrity();
    SaveGameObject->PlayerTransform = CurrentPestPlayer->GetTransform();
    SaveGameObject->bIsInCorrectZone = CurrentPestPlayer->GetArchetypeComponent()->IsInCorrectZone();
    SaveGameObject->SavedPestClass = CurrentPestPlayer->GetClass();
    //SaveGameObject->SavedPestChar = CurrentPestPlayer;
    SaveGameObject->MaxAbilities = CurrentPestPlayer->GetPestilenceAbilityComponent()->GetMaxAbilities();
    SaveGameObject->SavedPestCharID = CurrentPestPlayer->ID;
    APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
    SaveGameObject->bIntegrityUnlocked = GameMode->bIntegrityUnlocked;

    UPestilenceGameInstance* GameInstance = Cast<UPestilenceGameInstance>(GetGameInstance());
    SaveGameObject->SavedBlendParameters = GameInstance->ParameterArray;

    //Diary
    APestilenceHUD* PestilenceHUD = Cast<APestilenceHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (PestilenceHUD)
    {
        SaveGameObject->DiaryActiveEntrances = PestilenceHUD->DiaryMenuWidgetInstance->DiaryActiveEntrance;
    }

    SaveGameObject->SaveDate = FDateTime::Now();
    SaveGameObject->TotalPlayTime += GetWorld()->GetRealTimeSeconds() / 60.0f; // Save PlayTime in minutes

    SaveGameObject->FormattedInfo = FormatDateInfo(SaveGameObject->SaveDate, SaveGameObject->TotalPlayTime);

    const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, CurrentSlotName, 0);
    LogIfGameWasSavedOrNot(IsSaved);

    OnEndSave.Broadcast();
}

void UPestilenceSaveGameSubsystem::QuickLoad()
{
    //Broadcast
    OnLoad.Broadcast();
    ResetParameters();

    USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0);
    SaveGameObject = Cast<UPestilenceSaveGame>(LoadedGame);

    if (!SaveGameObject)
    {
        OnNoSaveFound.Broadcast();
    }
    else
    {
        bShouldLoad = true;
        LoadedSlot = CurrentSlotName;
        //Iterar world
        for (FActorIterator It(GetWorld()); It; ++It)      //for (FActorIterator It(GetWorld()); It; ++It)
        {
            AActor* Actor = *It;
            if (Actor->GetClass()->IsChildOf(APestilenceCharacter::StaticClass()))  //if (Actor->GetClass()->IsChildOf(APestilenceCharacter::StaticClass()))
            {
                for (FActorSaveData& ActorData : SaveGameObject->SavedActors)
                {
                    APestilenceCharacter* PActor = Cast<APestilenceCharacter>(Actor);
                    if (ActorData.CharacterID == PActor->ID)
                    {
                        if (Actor != GetWorld()->GetFirstPlayerController()->GetPawn())
                        {
                            PActor->bIsDead = ActorData.bIsDead;

                            if (PActor->bIsDead) { PActor->OnKill(); }
                        }
                    }
                }
            }
            else if (Actor->GetClass()->IsChildOf(ACheckPoint::StaticClass()))
            {
                for (FCheckPointSaveData& CheckPointData : SaveGameObject->SavedCheckPoints)
                {
                    ACheckPoint* CurrentCheckPoint = Cast<ACheckPoint>(Actor);
                    if (CheckPointData.CheckPointID == CurrentCheckPoint->ID)
                    {
                        CurrentCheckPoint->SetCheckPointState(CheckPointData.bIsConsumed);

                        Actor->SetActorHiddenInGame(CheckPointData.bIsConsumed);
                        Actor->SetActorEnableCollision(!CheckPointData.bIsConsumed);
                    }
                }
            }
            else
            {
                continue;
            }
        }

        //implementacion
        APestilenceCharacter* CurrentPestPlayer = Cast<APestilenceCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
        const FVector PlayerLocation = SaveGameObject->PlayerTransform.GetLocation();
        const FRotator PlayerRotation = FRotator(SaveGameObject->PlayerTransform.GetRotation());

        FActorSpawnParameters ActorSpawnParameters;
        ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        AActor* SpawnedActor = GetWorld()->SpawnActor(SaveGameObject->SavedPestClass, &PlayerLocation, &PlayerRotation, ActorSpawnParameters);
        APestilenceCharacter* SpawnedPestilenceCharacter = Cast<APestilenceCharacter>(SpawnedActor);
        SpawnedPestilenceCharacter->GetMixAndMatchComponent()->GenerateCharacter();
        SpawnedPestilenceCharacter->GetMixAndMatchComponent()->CreateMesh();

        const auto OtherPossessionComponent = SpawnedPestilenceCharacter->GetPestilencePossessionComponent();
        OtherPossessionComponent->SwitchState(OtherPossessionComponent->GetPossessedState());
        Cast<APlayerController>(CurrentPestPlayer->GetController())->Possess(SpawnedPestilenceCharacter);
        CurrentPestPlayer->GetPestilencePossessionComponent()->OnEndPossessEvent().Broadcast();
        OnSavedCharacterPossessed();

        CurrentPestPlayer->GetInteractorComponent()->Enable(false);
        SpawnedPestilenceCharacter->GetPestilencePossessionComponent()->SetCurrentIntegrity(SaveGameObject->SavedIntegrity);

        bShouldRemoveLoadingScreen = true;

        CheckStreamingVolume(SaveGameObject->PlayerTransform.GetTranslation());
        
        APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
        GameMode->SetMaxAbilities(SaveGameObject->MaxAbilities);
        GameMode->bIntegrityUnlocked = SaveGameObject->bIntegrityUnlocked;


        if (!SaveGameObject->SavedBlendParameters.IsEmpty())
        {
            UPestilenceGameInstance* GameInstance = Cast<UPestilenceGameInstance>(GetGameInstance());
            GameInstance->ParameterArray = SaveGameObject->SavedBlendParameters;
            UMaterialParameterCollectionInstance* ToonShaderParameterInstance = GetWorld()->GetParameterCollectionInstance(GameInstance->ToonShaderParameterCollection);

            for (int i = 0; i < SaveGameObject->SavedBlendParameters.Num(); ++i)
            {
                ToonShaderParameterInstance->SetScalarParameterValue(SaveGameObject->SavedBlendParameters[i].ParameterName,
                    SaveGameObject->SavedBlendParameters[i].CurrentValue);
            }
        }

        APestilenceHUD* PestilenceHUD = Cast<APestilenceHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
        if (PestilenceHUD)
        {
            PestilenceHUD->DiaryMenuWidgetInstance->ActiveDiaryMassive(SaveGameObject->DiaryActiveEntrances);
        }
    }

    if(LevelsToLoad == 0)
    {
      OnLevelLoaded();
    }

    if (SaveGameObject->bTutoDialogSeen)
      TutoDialogSeen.Broadcast();
    if (SaveGameObject->bHUBVastSeen)
      HUBVastSeen.Broadcast();
    if (SaveGameObject->bTutoVastKilledData)
      TutoVastKilledData.Broadcast();
    if (SaveGameObject->bAlmacenVastKilled)
      AlmacenVastKilled.Broadcast();

    OnEndLoad.Broadcast();
}

void UPestilenceSaveGameSubsystem::ReLoad()
{
    SetCheckPointSlotName();
    QuickLoad();
}

bool UPestilenceSaveGameSubsystem::IsCheckPointArrayEmpty()
{
    UPestilenceSaveGame* LoadedSaveGame = Cast<UPestilenceSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
    if (LoadedSaveGame)
    {
        if (LoadedSaveGame->SavedCheckPoints.IsEmpty()) { return true; }
        else
        {
            return false;
        }
    }
    return true;
}

void UPestilenceSaveGameSubsystem::OnSavedCharacterPossessed()
{
  APestilenceCharacter* Player = Cast<APestilenceCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
  if (!ensureMsgf(Player, TEXT("Player does not exist"))) return;

  Player->bUseControllerRotationYaw = 0;
  Player->SetActorTransform(SaveGameObject->PlayerTransform);
  APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
  GameMode->GetNPCManager()->EnableNPC(Player, nullptr, true, 1);
  Player->bUseControllerRotationYaw = 1;
  Player->GetInteractorComponent()->Enable(false);
  Player->GetInteractorComponent()->Enable(true);

}

void UPestilenceSaveGameSubsystem::ResetParameters()
{
  bShouldLoad = false;
  SavedSwingDoors.Empty();
}

void UPestilenceSaveGameSubsystem::LogIfGameWasSavedOrNot(const bool IsSaved)
{
    if (IsSaved)
    {
        OnSuccesfulSave.Broadcast();
    }
    else
    {
        OnFailedSave.Broadcast();
    }
}

// You'll never catch me, not when I shift into maximum ñaping -> noway
void UPestilenceSaveGameSubsystem::CheckStreamingVolume(const FVector& _Location)
{
  TArray<FHitResult> Results;
  const FVector Start = SaveGameObject->PlayerTransform.GetTranslation();
  const FVector End = Start + FVector::ForwardVector * 0.001f;
  FCollisionQueryParams CollisionParams;
  LevelsToLoad = 0;
  LevelsLoaded = 0;
  StreamedLevels.Empty();

  GetWorld()->LineTraceMultiByChannel(Results, Start, End, ECC_STREAMINGVOLUME);
  for (int i = 0; i < Results.Num(); ++i)
  {
    ALevelStreamingVolume* LevelStreamingVolume = Cast<ALevelStreamingVolume>(Results[i].GetActor());
    if (!LevelStreamingVolume) continue;

    TArray<FName> LevelNames = LevelStreamingVolume->StreamingLevelNames;
    for (int j = 0; j < LevelNames.Num(); ++j)
    {
      ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(this, LevelNames[j]);
      if (!LevelStreaming->IsLevelVisible() && !LevelStreaming->OnLevelShown.IsAlreadyBound(this, &UPestilenceSaveGameSubsystem::OnLevelLoaded))
      {
        UGameplayStatics::SetGlobalTimeDilation(this, LoadTimeDilation);
        ++LevelsToLoad;
        LevelStreaming->OnLevelShown.AddDynamic(this, &UPestilenceSaveGameSubsystem::OnLevelLoaded);
        StreamedLevels.Add(LevelStreaming);
      }
    }
  }

  FTimerDelegate TimerDelegate;
  GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
  {
    if (LevelsLoaded < LevelsToLoad)
    {
      LevelsLoaded = LevelsToLoad;
      OnLevelLoaded();
    }
  }, LoadFailsafeTime * LoadTimeDilation, false);
}

FString UPestilenceSaveGameSubsystem::FormatDateInfo(FDateTime& _SavedTime, float& _PlayTime)
{
    FString DateStr = _SavedTime.ToHttpDate(); //Try ToFormattedString(TEXT("dd/MM/yyyy HH:mm"))
    

    int32 TotalPlayTimeHours = FMath::FloorToInt(_PlayTime / 60);
    int32 TotalPlayTimeMinutes = FMath::FloorToInt(FMath::Fmod(_PlayTime, 60));

    //FString DateInfo = FString::Printf(TEXT("Date : %s - Playtime : %02d:%02d"), *DateStr, TotalPlayTimeHours, TotalPlayTimeMinutes);
    FString DateInfo = DateStr.Replace(TEXT("GMT"), TEXT(""));

    return DateInfo;
}

void UPestilenceSaveGameSubsystem::OnLevelLoaded()
{
  ++LevelsLoaded;
  if(LevelsLoaded >= LevelsToLoad)
  {
    APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
    if (!SaveGameObject->bIsInCorrectZone)
    {
      TArray<APestilenceCharacter*> NPCs = GameMode->GetNPCManager()->GetNpcs();
      for (APestilenceCharacter* NPC : NPCs)
      {
        if (!IsValid(NPC)) continue;
        APestilenceAIController* AIController = NPC->GetControllerAI();
        if (!AIController) continue;
        AIController->SetIsInForbiddenZone(true);
      }
    }

    APestilenceCharacter* CurrentPestPlayer = Cast<APestilenceCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
    CurrentPestPlayer->bUseControllerRotationYaw = 0;
    CurrentPestPlayer->SetActorTransform(SaveGameObject->PlayerTransform);
    CurrentPestPlayer->bUseControllerRotationYaw = 1;
    CurrentPestPlayer->GetInteractorComponent()->Enable(true);
    UGameplayStatics::SetGlobalTimeDilation(this, 1.f);

    for(int i = 0; i < StreamedLevels.Num(); ++i)
    {
      if (StreamedLevels[i]->OnLevelShown.IsAlreadyBound(this, &UPestilenceSaveGameSubsystem::OnLevelLoaded))
        StreamedLevels[i]->OnLevelShown.RemoveDynamic(this, &UPestilenceSaveGameSubsystem::OnLevelLoaded);
    }

    if (!bShouldRemoveLoadingScreen) return;

    UPestilenceGameInstance* GameInstance = Cast<UPestilenceGameInstance>(GetGameInstance());
    GameInstance->RemoveLoadingScreen();

  }
}
