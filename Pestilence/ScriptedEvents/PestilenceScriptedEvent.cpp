// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceScriptedEvent.h" 	
#include "Engine/LevelStreaming.h"
#include "../AI/PestilenceTargetPoint.h"
#include "UObject/NameTypes.h"
#include "Kismet/GameplayStatics.h"

/*
void APestilenceScriptedEvent::LoadSublevels()
{
  for (int i = 0; i < SublevelsToLoad.Num(); ++i)
  {
    if(SublevelsToLoad[i] != "")
    {
      FLatentActionInfo LatentInfo;
      UGameplayStatics::LoadStreamLevel(this, SublevelsToLoad[i], true, true, LatentInfo);
    }
  }
}

void APestilenceScriptedEvent::UnloadSublevels()
{
  for (int i = 0; i < SublevelsToUnload.Num(); ++i)
  {
    if (SublevelsToUnload[i] != "")
    {
      FLatentActionInfo LatentInfo;
      UGameplayStatics::UnloadStreamLevel(this, SublevelsToUnload[i], LatentInfo, true);
    }
  }
}
*/

void APestilenceScriptedEvent::ActivateTargetPoints()
{
  for (int i = 0; i < TargetPointsToActivate.Num(); ++i)
  {
    TargetPointsToActivate[i]->SetPointActive(true);
  }
}

void APestilenceScriptedEvent::DeactivateTargetPoints()
{
  for (int i = 0; i < TargetPointsToDeactivate.Num(); ++i)
  {
    TargetPointsToDeactivate[i]->SetPointActive(false);
  }
}
