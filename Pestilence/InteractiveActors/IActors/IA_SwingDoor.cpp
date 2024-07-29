#include "Pestilence/InteractiveActors/IActors/IA_SwingDoor.h"

#include "Pestilence/Widgets/LockpickWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "../../Components/Interaction/InteractorComponent.h"
#include "../../Components/Interaction/InteractableComponent.h"
#include "Components/BoxComponent.h"
#include "SmartObjectComponent.h"
#include "NavAreas/NavArea_Null.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/AI/PestilenceNavLinkComponent.h"
#include "Pestilence/Animation/PestilenceHands/PestilenceHandsAnimInstance.h"
#include "../../Enums/SuspiciousActionType.h"
#include "../../Components/Perception/SuspiciousActionSenderComponent.h"
#include "Pestilence/AI/NavArea/NavArea_Interact.h"
#include "Components/ArrowComponent.h"
#include "../../Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/Core/PestilenceGameInstance.h"
#include "Pestilence/Core/PestilenceSaveGameSubsystem.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"

AIA_SwingDoor::AIA_SwingDoor(const FObjectInitializer& ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    // Set the initial state of the door
    bOpen = false;
    bReadyState = true;

    // Create a door frame and make it the root component of the actor
    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
    RootComponent = DoorFrame;

    // Create a door and attach it to the door frame
    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    Door->SetupAttachment(RootComponent);

    Handle = CreateDefaultSubobject<USceneComponent>(TEXT("Handle"));
    Handle->SetupAttachment(Door);

    LockpickPosFront = CreateDefaultSubobject<USceneComponent>(TEXT("Lockpick Pos Front"));
    LockpickPosFront->SetupAttachment(RootComponent);
    LockpickArrowFront = CreateDefaultSubobject<UArrowComponent>(TEXT("Lockpick Arrow Front"));
    LockpickArrowFront->SetupAttachment(LockpickPosFront);

    LockpickPosBack = CreateDefaultSubobject<USceneComponent>(TEXT("Lockpick Pos Back"));
    LockpickPosBack->SetupAttachment(RootComponent);
    LockpickArrowBack = CreateDefaultSubobject<UArrowComponent>(TEXT("Lockpick Arrow Back"));
    LockpickArrowBack->SetupAttachment(LockpickPosBack);

    EndLockpickPosFront = CreateDefaultSubobject<USceneComponent>(TEXT("End Lockpick Pos Front"));
    EndLockpickPosFront->SetupAttachment(RootComponent);
    EndLockpickArrowFront = CreateDefaultSubobject<UArrowComponent>(TEXT("End Lockpick Arrow Front"));
    EndLockpickArrowFront->SetupAttachment(EndLockpickPosFront);

    EndLockpickPosBack = CreateDefaultSubobject<USceneComponent>(TEXT("End Lockpick Pos Back"));
    EndLockpickPosBack->SetupAttachment(RootComponent);
    EndLockpickArrowBack = CreateDefaultSubobject<UArrowComponent>(TEXT("End Lockpick Arrow Back"));
    EndLockpickArrowBack->SetupAttachment(EndLockpickPosBack);

    InteractableCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Interactable Collider"));
    InteractableCollider->SetupAttachment(Door);

    InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
    InteractableComponent->InteractableType = EInteractableType::Door;

    /** Set Link Navigation, to AI use doors
    * Get the defined areas that we will use */
    InteractNavArea = UNavArea_Interact::StaticClass();
    NullNavArea = UNavArea_Null::StaticClass();

    PestilenceNavLinkComponent = CreateDefaultSubobject<UPestilenceNavLinkComponent>(TEXT("NavLinkComponent"));
    /** NavLink Points, Start, End & Direction */
    PestilenceNavLinkComponent->SetLinkData(LinkStart, LinkEnd, ENavLinkDirection::BothWays);
    PestilenceNavLinkComponent->SetEnabledArea(InteractNavArea);
    PestilenceNavLinkComponent->AddNavigationObstacle(NullNavArea, ObstacleExtent, ObstacleOffset);

    SmokeParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Smoke Particles"));
    SmokeParticles->SetupAttachment(Door);

    ImpactParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Impact Particles"));
    ImpactParticles->SetupAttachment(Door);
}

void AIA_SwingDoor::BeginPlay()
{
  Super::BeginPlay();

  InitialYawRotation = Door->GetComponentRotation().Yaw;
  InteractableComponent->InteractableType = EInteractableType::Door;

  if (bHasMinigame)
  { 
    bLocked = true;

    InteractableComponent->InteractableType = EInteractableType::LockedDoor;
  } // Moved Here for savereasons
  
  CheckDoorSave();
    
  RotateValue = 1.0f; 

  if (DoorCurve)
  {
    float DoorCurveMinTime, DoorCurveMaxTime;
    DoorCurve->GetTimeRange(DoorCurveMinTime, DoorCurveMaxTime);
    DoorTime = DoorCurveMaxTime - DoorCurveMinTime;

    if (bStartOpen)
    {
      SetOpenInmediate();
    }
  }
  if (GateCurve)
  {
    float GateCurveMinTime, GateCurveMaxTime;
    GateCurve->GetTimeRange(GateCurveMinTime, GateCurveMaxTime);
    GateTime = GateCurveMaxTime - GateCurveMinTime;

  }
  if(bGate)
  {
    InteractableComponent->InteractableType = EInteractableType::BlockedDoor;
  }

  InteractableComponent->Initialize(InteractableCollider);
  InteractableComponent->OnInteracted.AddDynamic(this, &AIA_SwingDoor::OnInteracted);
  ReStart.AddDynamic(this, &AIA_SwingDoor::RestartMiniGame);
}

void AIA_SwingDoor::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (bUpdateDoor)
  {
    DoorTimer += bDoorReverse ? -DeltaTime : DeltaTime;
    ControlDoor(DoorTimer, DoorCurve);
    bUpdateDoor = bDoorReverse ? DoorTimer > 0 : DoorTimer <= DoorTime;
    bReadyState = !bUpdateDoor;

    if(bReadyState && !bOpen)
    {
      const FVector NewPawnLocation = RotateValue < 0 ? EndLockpickPosFront->GetComponentLocation() : EndLockpickPosBack->GetComponentLocation();
      CheckPawns(NewPawnLocation);
    }
  }
  if (bUpdateGate)
  {
    GateTimer += DeltaTime;
    ControlDoor(GateTimer, GateCurve);
    bUpdateGate = GateTimer <= GateTime;
    bReadyState = !bUpdateGate;
  }

  if(bPickingDoor)
  {
    PickingDoorNotifyTimer += DeltaTime;
    if(PickingDoorNotifyTimer > PickingDoorNotifyTime)
    {
      PickingDoorNotifyTimer -= PickingDoorNotifyTime;
      if (Interactor)
        Interactor->GetSuspiciousActionSenderComponent()->SubmitSuspiciousAction(Interactor, ESuspiciousActionType::PickDoor);
    }
  }
}

void AIA_SwingDoor::OnTranslating(bool _bIsTranslating, UPestilenceMovementComponent* _MovementComponent)
{
  if(!_bIsTranslating && bPickingDoor)
  {
    _MovementComponent->LimitLook(LockpickLookAngleLimit);
    _MovementComponent->OnTranslating.RemoveDynamic(this, &AIA_SwingDoor::OnTranslating);
  }
}

void AIA_SwingDoor::ControlDoor(float _Timer, UCurveFloat* _Curve)
{
  float CurveFloatValue = RotateValue * _Curve->GetFloatValue(_Timer);
  FRotator NewRotation = FRotator(0.f, CurveFloatValue, 0.f);

  Door->SetRelativeRotation(NewRotation);
}

void AIA_SwingDoor::SetState()
{
    // Set the ready state of the door to true, indicating that it's ready to be interacted with again
    bReadyState = true; 
}

void AIA_SwingDoor::OnInteracted(bool bResult, UInteractorComponent* Sender)
{
  if(!IsValid(Sender)) return;
  APestilenceCharacter* PestilenceCharacter = Sender->GetOwner<APestilenceCharacter>();
  if (!PestilenceCharacter) return;
  
  if(Sender->GetOwner() != UGameplayStatics::GetPlayerPawn(this, 0))
    DoorLogic(Sender);
  else
  {
      if (bGate)
      {
          OneWayGateLogic(Sender);
      }
      else if (bLocked && bHasMinigame)
      {
          Interactor = Cast<APestilenceCharacter>(Sender->GetOwner());
          TriggerLockpickMinigame();
      }
      else
      {
          DoorLogic(Sender);
      }
  }
}


void AIA_SwingDoor::DoorLogic(UInteractorComponent* Sender)
{
  APestilenceCharacter* PestilenceCharacter = Sender->GetOwner<APestilenceCharacter>();
  if (!IsValid(PestilenceCharacter)) return;
  
  UPestilenceMovementComponent* MovementComponent = PestilenceCharacter->GetPestilenceMovementComponent();
  if (!IsValid(MovementComponent)) return;
  
  bOpen = !bOpen; 
  if (bOpen) 
  {
    InteractableComponent->InteractableType = EInteractableType::OpenDoor;
    if (bDynamic)
    {
      float Projection = FVector::DotProduct(Sender->GetOwner()->GetActorForwardVector(), GetActorForwardVector());
      RotateValue = Projection > 0.0f ? -1.f : 1.f;
    }
    else
      RotateValue = 1.0f;

    if (DoorOpeningSound) { UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorOpeningSound, GetActorLocation(), 1.0f, 1.0f, 0.0f); }
  }
  else
  {
    InteractableComponent->InteractableType = EInteractableType::Door;
    if (DoorClosingSound) { UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorClosingSound, GetActorLocation(), 1.0f, 1.0f, 0.0f); }
    
    const FVector NewPawnLocation = RotateValue < 0 ? EndLockpickPosFront->GetComponentLocation() : EndLockpickPosBack->GetComponentLocation();
    CheckPawns(NewPawnLocation);
  }
  bReadyState = false;
  bUpdateDoor = true;
  bDoorReverse = !bOpen;

  UpdateNavMesh();
  MovementComponent->StartUsingDoor(true, InitialYawRotation,Door->GetComponentRotation().Yaw);
}

void AIA_SwingDoor::OneWayGateLogic(UInteractorComponent* Sender)
{
  APestilenceCharacter* PestilenceCharacter = Sender->GetOwner<APestilenceCharacter>();
  if (!IsValid(PestilenceCharacter)) return;

  UPestilenceMovementComponent* MovementComponent = PestilenceCharacter->GetPestilenceMovementComponent();
  if (!IsValid(MovementComponent)) return;
  
  float Projection = FVector::DotProduct(Sender->GetOwner()->GetActorForwardVector(), GetActorForwardVector());
  
  if (Projection > 0)
  {
    if (DoorUnlockSound) { UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorUnlockSound, GetActorLocation(), 1.0f, 1.0f, 0.0f); }
  }
  else
  {
    if (GateWrongSideSound) { UGameplayStatics::PlaySoundAtLocation(GetWorld(), GateWrongSideSound, GetActorLocation(), 1.0f, 1.0f, 0.0f); }
  }
  bGate = Projection <= 0;
  bUpdateGate = bGate;
  GateTimer = 0.f;
  if (!bGate)
  {
    OnUnlocked.Broadcast();
    InteractableComponent->InteractableType = EInteractableType::Door;
  }

  MovementComponent->StartUsingDoor(true, InitialYawRotation, Door->GetComponentRotation().Yaw);
}

void AIA_SwingDoor::SetOpenInmediate()
{
  bDoorReverse = true;
  bOpen = true;
  ControlDoor(DoorTime, DoorCurve);
  DoorTimer = DoorTime;
  InteractableComponent->InteractableType = EInteractableType::OpenDoor;
  UpdateNavMesh();
}

void AIA_SwingDoor::SetClosedInmediate()
{
  bDoorReverse = false;
  bOpen = false;
  ControlDoor(0.f, DoorCurve);
  DoorTimer = 0.f;
  if (bGate)
    InteractableComponent->InteractableType = EInteractableType::BlockedDoor;
  else if (bLocked)
    InteractableComponent->InteractableType = EInteractableType::LockedDoor;
  else
    InteractableComponent->InteractableType = EInteractableType::Door;

  UpdateNavMesh();
}

void AIA_SwingDoor::TriggerLockpickMinigame()
{
    ReStart.Broadcast();

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
      APestilenceCharacter* pCharacter = Cast<APestilenceCharacter>(PlayerController->GetCharacter());
      if (pCharacter)
      {
        pCharacter->ChangeToLockpinkMappingContext();
        USceneComponent* Target = FVector::DotProduct(pCharacter->GetActorForwardVector(), GetActorForwardVector()) > 0.f ? LockpickPosFront : LockpickPosBack;
        pCharacter->GetPestilenceMovementComponent()->StartTranslation(Target->GetComponentLocation(), Target->GetComponentRotation(), LockpickHalfHeight);
        pCharacter->GetPestilenceMovementComponent()->OnTranslating.AddDynamic(this, &AIA_SwingDoor::OnTranslating);

        pCharacter->GetInteractorComponent()->Enable(false);
      }
    }
    EnableIdleBorderDoorknob();
    if(Interactor)
      Interactor->GetSuspiciousActionSenderComponent()->SubmitSuspiciousAction(Interactor, ESuspiciousActionType::PickDoor);
    bPickingDoor = true;

    StartSmokeParticles();
    FVector RelativeLocation = ImpactParticles->GetRelativeLocation();
    RelativeLocation.X = FVector::DotProduct(Interactor->GetActorForwardVector(), GetActorForwardVector()) > 0.f ? -FMath::Abs(RelativeLocation.X) : FMath::Abs(RelativeLocation.X);
    ImpactParticles->SetRelativeLocation(RelativeLocation);

    RelativeLocation = SmokeParticles->GetRelativeLocation();
    RelativeLocation.X = FVector::DotProduct(Interactor->GetActorForwardVector(), GetActorForwardVector()) > 0.f ? -FMath::Abs(RelativeLocation.X) : FMath::Abs(RelativeLocation.X);
    SmokeParticles->SetRelativeLocation(RelativeLocation);
}

void AIA_SwingDoor::UnlockDoorSuccess()
{
  if (bLocked)
  {
    bLocked = false;
   
    FTimerDelegate TimerDelegate;

    GetWorld()->GetTimerManager().SetTimer(MappingContextHandle, [this]()
      {
        DelayContext();
      }, ChangeContextDelay, false);

      if (DoorUnlockSound) { UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorUnlockSound, GetActorLocation(), 1.0f, 1.0f, 0.0f); }
      //GetWorldTimerManager().SetTimer(DelayTimerHandle, FTimerDelegate::CreateLambda([this]()
      //{
      //        //bLocked...
      //}), 0.5f, false);

    if (Interactor)
    {
      Interactor->GetSuspiciousActionSenderComponent()->SubmitSuspiciousAction(Interactor, ESuspiciousActionType::PickDoor);

      USceneComponent* Target = FVector::DotProduct(Interactor->GetActorForwardVector(), GetActorForwardVector()) > 0.f ? EndLockpickPosFront : EndLockpickPosBack;
      Interactor->GetPestilenceMovementComponent()->StartTranslation(Target->GetComponentLocation(), Target->GetComponentRotation());
      Interactor->GetPestilenceMovementComponent()->UnlimitLook();
      Interactor->GetInteractorComponent()->Enable(true);
    }

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
      APestilenceCharacter* pCharacter = Cast<APestilenceCharacter>(PlayerController->GetCharacter());
      if (pCharacter)
      {
        pCharacter->ChangeToDefaultMappingContext();
      }
    }
    bPickingDoor = false;
    
  }
  OnUnlocked.Broadcast(); 
  DisableIdleBorder(); 
  StopSmokeParticles();
  InteractableComponent->InteractableType = EInteractableType::Door;
}

void AIA_SwingDoor::UnlockDoorFailure_Implementation()
{
    FTimerDelegate TimerDelegate;

    GetWorld()->GetTimerManager().SetTimer(MappingContextHandle, [this]()
      {
        DelayContext();
      }, ChangeContextDelay, false);

    if (Interactor)
    {
      USceneComponent* Target = FVector::DotProduct(Interactor->GetActorForwardVector(), GetActorForwardVector()) > 0.f ? EndLockpickPosFront : EndLockpickPosBack;
      Interactor->GetPestilenceMovementComponent()->StartTranslation(Target->GetComponentLocation(), Target->GetComponentRotation());
      Interactor->GetPestilenceMovementComponent()->UnlimitLook();
      //Interactor->GetInteractorComponent()->Enable(true);
    }

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
      APestilenceCharacter* pCharacter = Cast<APestilenceCharacter>(PlayerController->GetCharacter());
      if (pCharacter)
      {
        pCharacter->ChangeToDefaultMappingContext();
      }
    }

    bPickingDoor = false;
    DisableIdleBorder();
    StopSmokeParticles();
    UnlockDoorFailureBP();
}

void AIA_SwingDoor::UpdateNavMesh()
{
  if (bOpen && !bLocked)
  {
    PestilenceNavLinkComponent->ClearNavigationObstacle();
    PestilenceNavLinkComponent->SetEnabled(false);
  }
  else if (!bOpen && !bLocked)
  {
    PestilenceNavLinkComponent->AddNavigationObstacle(NullNavArea, ObstacleExtent, ObstacleOffset);
    PestilenceNavLinkComponent->SetEnabled(true);
  }
  else if (bLocked)
  {
    PestilenceNavLinkComponent->AddNavigationObstacle(NullNavArea, ObstacleExtent, ObstacleOffset);
    PestilenceNavLinkComponent->SetEnabled(false);
  }
}

void AIA_SwingDoor::DelayContext()
{
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController)
  {
    APestilenceCharacter* pCharacter = Cast<APestilenceCharacter>(PlayerController->GetCharacter());
    if (pCharacter)
    {
      pCharacter->ChangeToDefaultMappingContext();
    }
  }
}

void AIA_SwingDoor::CheckPawns(const FVector& _NewPawnLocation)
{
  TArray<FHitResult> Results;
  const FVector Start = UKismetMathLibrary::TransformLocation(GetTransform(), InteractableCollider->GetRelativeLocation());
  const FVector End = Start + FVector::ForwardVector * 0.001f;
  const FQuat Rot = GetActorRotation().Quaternion();
  const FCollisionShape Box = FCollisionShape::MakeBox(InteractableCollider->GetScaledBoxExtent());
  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(this, 0));
  bool bHit = GetWorld()->SweepMultiByObjectType(Results, Start, End, Rot, ECC_Pawn, Box, CollisionParams);

  for(int i = 0; i < Results.Num(); ++i)
  {
    APestilenceCharacter* Character = Cast<APestilenceCharacter>(Results[i].GetActor());
    if (!Character) continue;

    Character->GetPestilenceMovementComponent()->StartTranslation(_NewPawnLocation, Character->GetActorRotation());
  }
}

void AIA_SwingDoor::EnableIdleBorderDoorknob()
{
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController)
  {
    APestilenceCharacter* pCharacter = Cast<APestilenceCharacter>(PlayerController->GetCharacter());
    if (pCharacter)
    {
      UAnimInstance* AnimInstance = pCharacter->GetMesh1P()->GetAnimInstance();
      if (AnimInstance) 
      {
        UPestilenceHandsAnimInstance* HandsAnimInstance = Cast<UPestilenceHandsAnimInstance>(AnimInstance);
        if (HandsAnimInstance) 
        {
          HandsAnimInstance->bIsInMinigameDoor = true;
        }
      }
    }
  }
}

void AIA_SwingDoor::DisableIdleBorder()
{
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController)
  {
    APestilenceCharacter* pCharacter = Cast<APestilenceCharacter>(PlayerController->GetCharacter());
    if (pCharacter)
    {
      UAnimInstance* AnimInstance = pCharacter->GetMesh1P()->GetAnimInstance();
      if (AnimInstance)
      {
        UPestilenceHandsAnimInstance* HandsAnimInstance = Cast<UPestilenceHandsAnimInstance>(AnimInstance);
        if (HandsAnimInstance)
        {
          HandsAnimInstance->bIsInMinigameDoor = false;
        }
      }
    }
  }
}

void AIA_SwingDoor::EnableDisintegrate()
{

}

void AIA_SwingDoor::EnableSuccess()
{
  StartImpactParticles();
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController)
  {
    APestilenceCharacter* pCharacter = Cast<APestilenceCharacter>(PlayerController->GetCharacter());
    UAnimInstance* AnimInstance = pCharacter->GetMesh1P()->GetAnimInstance();
    if (AnimInstance)
    {
      UPestilenceHandsAnimInstance* HandsAnimInstance = Cast<UPestilenceHandsAnimInstance>(AnimInstance);
      if (HandsAnimInstance)
      {
        HandsAnimInstance->bIsSuccessMinigameDoor = true;
      }
    }
  }
}

void AIA_SwingDoor::EnableFail()
{
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController)
  {
    APestilenceCharacter* pCharacter = Cast<APestilenceCharacter>(PlayerController->GetCharacter());
    if (pCharacter)
    {
      UAnimInstance* AnimInstance = pCharacter->GetMesh1P()->GetAnimInstance();
      if (AnimInstance)
      {
        UPestilenceHandsAnimInstance* HandsAnimInstance = Cast<UPestilenceHandsAnimInstance>(AnimInstance);
        if (HandsAnimInstance)
        {
          HandsAnimInstance->bIsFailMinigameDoor = true;
        }
      }
    }
  }
}

void AIA_SwingDoor::StartSmokeParticles()
{
  // SmokeParticles->ActivateSystem();
}

void AIA_SwingDoor::StopSmokeParticles()
{
  // SmokeParticles->DeactivateImmediate();
}

void AIA_SwingDoor::StartImpactParticles()
{
  ImpactParticles->ActivateSystem();
}

void AIA_SwingDoor::StopImpactParticles()
{
  ImpactParticles->DeactivateImmediate();
}

void AIA_SwingDoor::SetLockedDoor(bool _bLocked)
{
  bLocked = _bLocked;
  bHasMinigame = _bLocked;
  
  if (_bLocked)
  {
    InteractableComponent->InteractableType = EInteractableType::LockedDoor;
  }
  else 
  {
    InteractableComponent->InteractableType = EInteractableType::Door;
  }
}

// https://static.wikia.nocookie.net/pizzatower/images/7/79/Spr_comboend_title1_28.gif
// Si se guardan las puertas en el archivo de guardado directamente al destruirse estás guardando algo que no se ha pedido que se guarde
// sea por checkpoint o por guardado manual en un fichero en el que no podría corresponder. Si guardo manualmente en el fichero 3, abro la puerta
// y esta se destruye porque se descarga el subnivel, cuando vuelva a cargar el fichero 3 aparecerá abierta aunque no haya hecho nada aún. 
// Mejor tenerla guardada en algún sitio persistente durante el nivel y ya al guardar por checkpoint o manual te lo guardas en el archivo. 
// Y claro, al terminar la sesión de juego también se guarda aunque no debería así que testear en el editor... xd
//  -> Yep, como se planteo es que se guardara info de este tipo al descargar los sublevels por si el player tenia que volver a una zona y al
//  seguir con el manager de saves lo deje tirado, tendria que haber registrado las minutas de la reunion con Alberto, pero por lo que veo
//  ahora esta como estaba en un principio, lo que me extraña es lo del editor por las pruebas que hice en su dia pero por lo que he visto 
//  bastantes partes del systema se han fixeado
void AIA_SwingDoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  if (EndPlayReason == EEndPlayReason::RemovedFromWorld)
  {
    UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
    UPestilenceSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();

    FDoorSaveData* ExistingDoorData = nullptr;
    for (FDoorSaveData& ActorData : SaveSubsystem->SavedSwingDoors)
    {
      if (ActorData.DoorID == ID)
      {
        ExistingDoorData = &ActorData;
        break;
      }
    }

    if (ExistingDoorData)  // If the actor exists in the array, update its information
    {
      ExistingDoorData->bIsLocked = bLocked;
      ExistingDoorData->bIsOpen = bOpen;
    }
    else // Actor doesn't exist, create and add its information
    {
      FDoorSaveData SwingDoorData;
      SwingDoorData.DoorID = ID;
      SwingDoorData.bIsLocked = bLocked;
      SwingDoorData.bIsOpen = bOpen;

      SaveSubsystem->SavedSwingDoors.Add(SwingDoorData);
    }
  }
}

void AIA_SwingDoor::CheckDoorSave()
{
  UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
  UPestilenceSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
  UPestilenceSaveGame* LoadedSaveGame = nullptr;

  if(SaveSubsystem->SavedSwingDoors.Num() > 0)
  {
    for (FDoorSaveData& DoorData : SaveSubsystem->SavedSwingDoors)
    {
      if (DoorData.DoorID == ID)
      {
        bHasMinigame = DoorData.bIsLocked;
        bLocked = DoorData.bIsLocked;
        bOpen = DoorData.bIsOpen;
        if (DoorData.bIsOpen) // && GameInstance->bLoadGame
        {
          bStartOpen = DoorData.bIsOpen;
        }
        return;
      }
    }
  }

  if (!SaveSubsystem->bShouldLoad || !bShouldSaveState) return;
  if (UGameplayStatics::DoesSaveGameExist(SaveSubsystem->LoadedSlot, 0))
  {
    LoadedSaveGame = Cast<UPestilenceSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSubsystem->LoadedSlot, 0));
  }

  if (LoadedSaveGame)
  {
    for (FDoorSaveData& DoorData : LoadedSaveGame->SavedSwingDoors)
    {
      if (DoorData.DoorID == ID)
      {
        bHasMinigame = DoorData.bIsLocked;
        bLocked = DoorData.bIsLocked;
        bOpen = DoorData.bIsOpen;
        if (DoorData.bIsOpen) // && GameInstance->bLoadGame
        {
            bStartOpen = DoorData.bIsOpen;
        }
        return;
      }
    }
  }
}

void AIA_SwingDoor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    if (!ID.IsValid())
    {
        ID = FGuid::NewGuid();
        MarkPackageDirty();
    }
}
