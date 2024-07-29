#include "Pestilence/Core/CheckPoint.h"
#include <Components/BoxComponent.h>
#include "Components/ArrowComponent.h"
#include "Pestilence/Core/PestilenceSaveGameSubsystem.h"
#include "Pestilence/Core/PestilenceSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "PestilenceGameMode.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = CollisionBox;

	SpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnLocation"));
	SpawnLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (CollisionBox)
	{
		FVector BoxExtent = CollisionBox->GetScaledBoxExtent();
		SpawnLocation->SetRelativeLocation(FVector(BoxExtent.X / 2, BoxExtent.Y / 2, BoxExtent.Z / 2));
	}

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapBegin);
}

void ACheckPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!ID.IsValid())
	{
		ID = FGuid::NewGuid();
		MarkPackageDirty();
	}
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
}

void ACheckPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorld()->GetFirstPlayerController()->GetPawn() == OtherActor)
	{
		APestilenceGameMode* GameMode = Cast<APestilenceGameMode>(GetWorld()->GetAuthGameMode());

    // o pones return o niegas la condicion pero no pongas if{} else 😭
    // para negar una condición si es or le pones and y niegas ambas
    // (!bConsumed && !GameMode->GetChaseState()) 
    //     '-> Esto es mejor que la captura que paso josemi, ty
    if (bConsumed || GameMode->GetChaseState()) return;

    bConsumed = !bConsumed;

    UGameInstance* GameInstance = GetGameInstance();
    UPestilenceSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
    SaveSubsystem->SetCheckPointSlotName();
    {
      UPestilenceSaveGame* CheckpointSave = Cast<UPestilenceSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSubsystem->GetCurrentSlotName(), 0));
      if (CheckpointSave && CheckpointSave->CheckpointPriority > CheckpointPriority) return;
    }

    SaveSubsystem->ResetSlotName();
    if (!UGameplayStatics::DoesSaveGameExist(SaveSubsystem->GetCurrentSlotName(), 0))
    {
      SaveSubsystem->QuickSave();
    }

    SaveSubsystem->SetCheckPointSlotName();
    SaveSubsystem->QuickSave();


    UPestilenceSaveGame* CheckpointSave = Cast<UPestilenceSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSubsystem->GetCurrentSlotName(), 0));
    if (CheckpointSave)
    {
      CheckpointSave->CheckpointPriority = CheckpointPriority;

      FVector TempVect = CheckpointSave->PlayerTransform.GetLocation();
      TempVect.X = SpawnLocation->GetComponentLocation().X;
      TempVect.Y = SpawnLocation->GetComponentLocation().Y;
      TempVect.Z = SpawnLocation->GetComponentLocation().Z;

      CheckpointSave->PlayerTransform.SetLocation(TempVect);
      CheckpointSave->PlayerTransform.SetRotation(SpawnLocation->GetComponentRotation().Quaternion());

      if (bRestoreIntegrity && CheckpointSave->SavedIntegrity <= GameMode->IntegrityThreshold)
      {
        CheckpointSave->SavedIntegrity = GameMode->IntegritySetOnLoad;
      }

      UGameplayStatics::SaveGameToSlot(CheckpointSave, SaveSubsystem->GetCurrentSlotName(), 0);
    }

    SaveSubsystem->ResetSlotName();
	}
}
