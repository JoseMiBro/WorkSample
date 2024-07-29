// Copyright Epic Games, Inc. All Rights Reserved.

#include "PestilenceGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "../Components/Abilities/Pickable/PickableAbility.h"
#include "../Components/Abilities/PestilenceAbility.h"
#include "../Components/Abilities/ProjectilePredictionMesh.h"
#include "Pestilence/Characters/NPCManager.h"

APestilenceGameMode::APestilenceGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Pestilence/Characters/FirstPerson/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

    OnAIBeginChase.AddDynamic(this, &APestilenceGameMode::HandleAIBeginChase);
    OnAIEndChase.AddDynamic(this, &APestilenceGameMode::HandleAIEndChase);
}

UPickableAbility* APestilenceGameMode::GetPickableAbility()
{
  return PickableAbility;
}

TArray<UPestilenceAbility*> APestilenceGameMode::GetAbilities()
{
  return AbilityObjects;
}

TArray<AProjectilePredictionMesh*> APestilenceGameMode::GetProjectilePredictionMeshes()
{
  return ProjectilePredictionMeshes;
}
TArray<AProjectilePredictionMesh*> APestilenceGameMode::GetProjectilePredictionBounceMeshes()
{
  return ProjectilePredictionBounceMeshes;
}
AProjectilePredictionMesh* APestilenceGameMode::GetProjectilePredictionGoalMesh()
{
  return ProjectilePredictionGoalMesh;
}

void APestilenceGameMode::SetMaxAbilities(int _MaxAbilities)
{
  MaxAbilities = _MaxAbilities;
  if (MaxAbilities > AbilityObjects.Num())
    MaxAbilities = AbilityObjects.Num();
  if (MaxAbilities < 0)
    MaxAbilities = 0;

  OnMaxAbilitiesChanged.Broadcast(MaxAbilities);
}

int APestilenceGameMode::GetMaxAbilities() const
{
  return MaxAbilities;
}

void APestilenceGameMode::HandleAIBeginChase()
{
    CurrentGuardsChasing = (CurrentGuardsChasing >= 0) ? (CurrentGuardsChasing + 1) : 1;
}

void APestilenceGameMode::HandleAIEndChase()
{
    CurrentGuardsChasing = (CurrentGuardsChasing > 0) ? (CurrentGuardsChasing - 1) : 0;
}

ANPCManager* APestilenceGameMode::GetNPCManager()
{
  return NPCManager;
}

void APestilenceGameMode::BeginPlay()
{
  Super::BeginPlay();

  PickableAbility = NewObject<UPickableAbility>(this, PickableAbilityClass);
  PickableAbility->Initialize(GetWorld());
  for (int i = 0; i < Abilities.Num(); ++i)
  {
    if (Abilities[i])
    {
      AbilityObjects.Add(NewObject<UPestilenceAbility>(this, Abilities[i]));
      AbilityObjects[AbilityObjects.Num() - 1]->Initialize(GetWorld());
    }
  }

  for (int i = 0; i < MaxProjectilePredictionMeshes; ++i)
  {
    ProjectilePredictionMeshes.Add(GetWorld()->SpawnActor<AProjectilePredictionMesh>(ProjectilePredictionMeshClass));
  }
  for (int i = 0; i < MaxProjectilePredictionBounceMeshes; ++i)
  {
    ProjectilePredictionBounceMeshes.Add(GetWorld()->SpawnActor<AProjectilePredictionMesh>(ProjectilePredictionBounceMeshClass));
  }
  ProjectilePredictionGoalMesh = (GetWorld()->SpawnActor<AProjectilePredictionMesh>(ProjectilePredictionGoalMeshClass));

  NPCManager = GetWorld()->SpawnActor<ANPCManager>(NPCManagerClass);
}

void APestilenceGameMode::SetKeyboardControl()
{
  if (!bUsingController) return;

  bUsingController = false;
  OnInputMethodChanged.Broadcast(bUsingController);
}

void APestilenceGameMode::SetControllerControl()
{
  if (bUsingController) return;

  bUsingController = true;
  OnInputMethodChanged.Broadcast(bUsingController);
}
