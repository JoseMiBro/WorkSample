#include "Pestilence/Components/Possession/PestilenceVisionComponent.h"

#include "PestilencePossessionComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Components/CapsuleComponent.h"
#include "RagdollState.h"
#include "../../Characters/NPCManager.h"
#include "../../Core/PestilenceGameMode.h"

UPestilenceVisionComponent::UPestilenceVisionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Owner = GetOwner<APestilenceCharacter>();
}

void UPestilenceVisionComponent::BeginPlay()
{
    Super::BeginPlay();
    GetAllPestilenceCharacters();
    SetActive(false);
}

void UPestilenceVisionComponent::Activate(const bool bReset)
{
    Super::Activate(bReset);
    SetComponentTickEnabled(true);
    PestilenceVisionPerformedDelegate.Broadcast(true);

    SearchForCharactersInRange();
    TimeSinceLastSearch = 0.f;

    SearchLookingCharacter();
    ApplyMaterials();
}

void UPestilenceVisionComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TimeSinceLastSearch += DeltaTime;

    if (TimeSinceLastSearch >= RangeSearchFrec)
    {
        SearchForCharactersInRange();
        TimeSinceLastSearch = 0.f;
    }

    SearchLookingCharacter();
    ApplyMaterials();
}

void UPestilenceVisionComponent::Deactivate()
{
    Super::Deactivate();
    SetComponentTickEnabled(false);
    PestilenceVisionPerformedDelegate.Broadcast(false);
    RestoreSearchDefaults();

    if (!NPCManager)
    {
      GetAllPestilenceCharacters();
    }
    TArray<APestilenceCharacter*> NPCs = NPCManager->GetNpcs();

    for (APestilenceCharacter* PestilenceCharacter : NPCs)
    {
      if (!IsValid(PestilenceCharacter)) continue;
      PestilenceCharacter->SetOverlayMaterialNone();
    }
    
}

void UPestilenceVisionComponent::GetAllPestilenceCharacters()
{
  APestilenceGameMode* GameMode = Cast<APestilenceGameMode>(UGameplayStatics::GetGameMode(this));
  if(GameMode && GameMode->GetNPCManager())
  {
    NPCManager = GameMode->GetNPCManager();
  }
}

void UPestilenceVisionComponent::SearchForCharactersInRange()
{
    AvailableCharacters.Empty();

    if (!NPCManager)
    {
      GetAllPestilenceCharacters();
    }
    TArray<APestilenceCharacter*> NPCs = NPCManager->GetNpcs();
    for (APestilenceCharacter* Character : NPCs)
    {
        if (!Character || !Character->IsValidLowLevel() || Character == Owner ||
            Character->GetPestilencePossessionComponent()->GetCurrentState()->IsA(URagdollState::StaticClass())) continue;
        FVector CharacterLocation = Character->GetActorLocation();
        const float DistanceToSelf = FVector::DistSquared(CharacterLocation, Owner->GetFirstPersonCameraComponent()->GetComponentLocation());
        if (DistanceToSelf > DistanceToPossess * DistanceToPossess)
        {
            Character->SetOverlayMaterialNone();
            continue;
        }
        AvailableCharacters.Add(Character);
    }
}

bool UPestilenceVisionComponent::IsCharacterVisible(const APestilenceCharacter* Character)
{
    FHitResult WaistOutHit, HeadOutHit, FeetOutHit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    FVector WaistLocation = Character->GetActorLocation();
    FVector HeadLocation = WaistLocation + FVector(0, 0, CapsuleHalfHeight);
    FVector FeetLocation = WaistLocation - FVector(0, 0, CapsuleHalfHeight);
    // meter para que haga raycast a la cabeza y pies tambi�n
    const bool bIsWaistHit = GetWorld()->LineTraceSingleByChannel(
        WaistOutHit,
        Owner->GetFirstPersonCameraComponent()->GetComponentLocation(),
        WaistLocation,
        ECC_Visibility,
        QueryParams);
    const bool bIsHeadHit = GetWorld()->LineTraceSingleByChannel(
        HeadOutHit,
        Owner->GetFirstPersonCameraComponent()->GetComponentLocation(),
        HeadLocation,
        ECC_Visibility,
        QueryParams);
    const bool bIsFeetHit = GetWorld()->LineTraceSingleByChannel(
        FeetOutHit,
        Owner->GetFirstPersonCameraComponent()->GetComponentLocation(),
        FeetLocation,
        ECC_Visibility,
        QueryParams);

    if (!bIsWaistHit && !bIsHeadHit && !bIsFeetHit) return false;

    bool bIsCharacterVisible = false;
    if (bIsWaistHit)
        bIsCharacterVisible = bIsCharacterVisible || WaistOutHit.GetActor()->IsA(APestilenceCharacter::StaticClass());
    if (bIsHeadHit)
        bIsCharacterVisible = bIsCharacterVisible || HeadOutHit.GetActor()->IsA(APestilenceCharacter::StaticClass());
    if (bIsFeetHit)
        bIsCharacterVisible = bIsCharacterVisible || FeetOutHit.GetActor()->IsA(APestilenceCharacter::StaticClass());

    return bIsCharacterVisible;
}

void UPestilenceVisionComponent::SearchLookingCharacter()
{
    PrevLookingCharacter = LookingCharacter;
    const FVector CameraDir = Owner->GetFirstPersonCameraComponent()->GetForwardVector();
    float MinorAngle = TNumericLimits<float>::Max();
    for (const auto Character : AvailableCharacters)
    {
        if (!IsValid(Character)) continue;

        auto PossessionComponent = Character->GetPestilencePossessionComponent();

        if (PossessionComponent && !PossessionComponent->bCanBePossessed)
            continue;

        const FVector DirToCharacter = (Character->GetActorLocation() - Owner->GetFirstPersonCameraComponent()->GetComponentLocation()).GetSafeNormal();
        const float Angle = FMath::Acos(FVector::DotProduct(CameraDir, DirToCharacter));
        if (Angle < MinorAngle)
        {
            MinorAngle = Angle;
            LookingCharacter = Character;
        }
    }

    if (MinorAngle > MinimumAngleToLook || (LookingCharacter && !IsCharacterVisible(LookingCharacter)))
    {
      LookingCharacter = nullptr;
    }
    if(LookingCharacter != PrevLookingCharacter)
      OnPestilenceVisionTargetChangedDelegate.Broadcast(LookingCharacter);
}

void UPestilenceVisionComponent::RestoreSearchDefaults()
{
    AvailableCharacters.Empty();
    TimeSinceLastSearch = RangeSearchFrec;
    LookingCharacter = nullptr;
    PrevLookingCharacter = nullptr;
}

void UPestilenceVisionComponent::ApplyMaterials()
{
    for (APestilenceCharacter* AvailableCharacter : AvailableCharacters)
    {
        if (!IsValid(AvailableCharacter)) continue;
        
        auto PossessionComponent = AvailableCharacter->GetPestilencePossessionComponent();
        
        if (PossessionComponent && !PossessionComponent->bCanBePossessed)
        {
            AvailableCharacter->SetOverlayMaterialForbidden();
            continue;
        }

        if (AvailableCharacter == LookingCharacter)
        {
            AvailableCharacter->SetOverlayMaterialSelected();
            continue;
        }

        AvailableCharacter->SetOverlayMaterialDetected();
    }
}