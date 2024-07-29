#include "ArchetypeZone.h"

#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/Possession/ArchetypeComponent.h"
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "../Characters/NPCManager.h"
#include "../Core/PestilenceGameMode.h"

AArchetypeZone::AArchetypeZone()
{
  PrimaryActorTick.bCanEverTick = true;
}

void AArchetypeZone::BeginPlay()
{
  Super::BeginPlay();

  OnActorBeginOverlap.AddDynamic(this, &AArchetypeZone::OnActorEnter);
  OnActorEndOverlap.AddDynamic(this, &AArchetypeZone::OnActorExit);

  const TObjectPtr<APestilenceGameMode> GameMode = Cast<APestilenceGameMode>(UGameplayStatics::GetGameMode(this));
  if (GameMode && GameMode->GetNPCManager())
    NPCManager = GameMode->GetNPCManager();

  ShapeComponent = Cast<UShapeComponent>(GetComponentByClass(UShapeComponent::StaticClass()));
}

void AArchetypeZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  OnActorBeginOverlap.RemoveDynamic(this, &AArchetypeZone::OnActorEnter);
  OnActorEndOverlap.RemoveDynamic(this, &AArchetypeZone::OnActorExit);

  Super::EndPlay(EndPlayReason);
}

void AArchetypeZone::Refresh()
{
  FCollisionShape Shape;

  if (const USphereComponent* Sphere = Cast<USphereComponent>(ShapeComponent))
  {
    Shape = FCollisionShape::MakeSphere(Sphere->GetScaledSphereRadius());
  }
  else if (const UBoxComponent* Box = Cast<UBoxComponent>(ShapeComponent))
  {
    Shape = FCollisionShape::MakeBox(Box->GetScaledBoxExtent() / 2);
  }
  else
  {
    return;
  }

  if (!NPCManager)
  {
    APestilenceGameMode* GameMode = Cast<APestilenceGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode && GameMode->GetNPCManager())
      NPCManager = GameMode->GetNPCManager();
  }
  TArray<APestilenceCharacter*> NPCs = NPCManager->GetNpcs();
  for (APestilenceCharacter* NPC : NPCs)
  {
    if (!IsValid(NPC)) continue;
    APestilenceAIController* AIController = NPC->GetControllerAI();
    if (!AIController) continue;
    AIController->SetIsInForbiddenZone(false);
  }

  TArray<FHitResult> Results;
  const FVector Start = GetActorLocation();
  const FVector End = Start + FVector::ForwardVector * 0.001f;
  const FQuat Rot = GetActorRotation().Quaternion();

  if (GetWorld()->SweepMultiByObjectType(Results, Start, End, Rot, FCollisionObjectQueryParams::AllObjects, Shape))
    for (const FHitResult& Result : Results)
    {
      OnActorEnter(this, Result.GetActor());
    }
}

void AArchetypeZone::OnRefreshRequested()
{
  if (PrevPlayer)
  {
    PrevPlayer->GetPestilencePossessionComponent()->OnEndPossessEvent()
      .RemoveDynamic(this, &AArchetypeZone::OnRefreshRequested);
  }

  const auto Player = Cast<APestilenceCharacter>(
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

  if (IsValid(Player) && IsValid(Player->GetPestilencePossessionComponent()))
  {
    Player->GetPestilencePossessionComponent()->OnEndPossessEvent().AddDynamic(this, &AArchetypeZone::OnRefreshRequested);
    PrevPlayer = Player;
    Refresh();
  }
}

void AArchetypeZone::OnActorEnter(AActor* OverlappedActor, AActor* OtherActor)
{
  const auto* PestilenceCharacter = Cast<APestilenceCharacter>(OtherActor);
  if (!PestilenceCharacter ||
    !PestilenceCharacter->GetPestilencePossessionComponent()->IsPossessed() ||
    (!bForbiddenZone && (!RestrictedArchetypes.Contains(PestilenceCharacter->GetArchetypeComponent()->GetPestilenceArchetype()) &&
      !RestrictedGenders.Contains(PestilenceCharacter->GetArchetypeComponent()->GetPestilenceGender()))))
    return;

  PestilenceCharacter->GetArchetypeComponent()->ZoneChanged(false, bForbiddenZone);
  if (!NPCManager)
  {
    APestilenceGameMode* GameMode = Cast<APestilenceGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode && GameMode->GetNPCManager())
    {
      NPCManager = GameMode->GetNPCManager();
    }
  }
  TArray<APestilenceCharacter*> NPCs = NPCManager->GetNpcs();
  for (APestilenceCharacter* NPC : NPCs)
  {
    if (!IsValid(NPC)) continue;
    APestilenceAIController* AIController = NPC->GetControllerAI();
    if (!AIController) continue;
    AIController->SetIsInForbiddenZone(true);
  }

  UArchetypeComponent* ArchetypeComponent = PestilenceCharacter->GetArchetypeComponent();
  ArchetypeComponent->SetPreviousZone(ArchetypeComponent->GetCurrentZone());
  PestilenceCharacter->GetArchetypeComponent()->SetCurrentZone(this);
}

void AArchetypeZone::OnActorExit(AActor* OverlappedActor, AActor* OtherActor)
{
  const auto* PestilenceCharacter = Cast<APestilenceCharacter>(OtherActor);
  if (!PestilenceCharacter ||
    !PestilenceCharacter->GetPestilencePossessionComponent()->IsPossessed()
    || (PestilenceCharacter->GetArchetypeComponent()->GetCurrentZone() != this
      && !PestilenceCharacter->GetArchetypeComponent()->GetPreviousZone()))
    return;

  UArchetypeComponent* ArchetypeComponent = PestilenceCharacter->GetArchetypeComponent();
  
  if (ArchetypeComponent->GetPreviousZone())
  {
    if (ArchetypeComponent->GetPreviousZone() != this)
    {
      ArchetypeComponent->SetCurrentZone(ArchetypeComponent->GetPreviousZone());
      PestilenceCharacter->GetArchetypeComponent()->ZoneChanged(false, ArchetypeComponent->GetCurrentZone()->bForbiddenZone);
    }
    ArchetypeComponent->SetPreviousZone(nullptr);
  }
  else
  {
    ArchetypeComponent->ZoneChanged(true, false);

    if (!NPCManager)
    {
      APestilenceGameMode* GameMode = Cast<APestilenceGameMode>(UGameplayStatics::GetGameMode(this));
      if (GameMode && GameMode->GetNPCManager())
      {
        NPCManager = GameMode->GetNPCManager();
      }
    }
    TArray<APestilenceCharacter*> NPCs = NPCManager->GetNpcs();
    for (APestilenceCharacter* NPC : NPCs)
    {
      if (!IsValid(NPC)) continue;
      APestilenceAIController* AIController = NPC->GetControllerAI();
      if (!AIController) continue;
      AIController->SetIsInForbiddenZone(false);
    }
    PestilenceCharacter->GetArchetypeComponent()->SetCurrentZone(nullptr);
  }
}

bool AArchetypeZone::ActorIsRestricted(AActor* ActorToCheck) const
{
  const TObjectPtr<UArchetypeComponent> ArchetypeComponent = ActorToCheck->FindComponentByClass<UArchetypeComponent>();
  if (!IsValid(ArchetypeComponent)) return true;

  return RestrictedArchetypes.Contains(ArchetypeComponent->GetPestilenceArchetype()) ||
    RestrictedGenders.Contains(ArchetypeComponent->GetPestilenceGender());
}

void AArchetypeZone::Tick(float DeltaTime)
{
  if (FirstFrame)
  {
    OnRefreshRequested();
    FirstFrame = false;
  }

  Super::Tick(DeltaTime);
}
