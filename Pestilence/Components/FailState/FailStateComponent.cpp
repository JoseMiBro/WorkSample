#include "FailStateComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Camera/CameraComponent.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/Components/HUD/PestilencePauseComponent.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Abilities/PestilenceAbilityComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "../Possession/PestilencePossessionComponent.h"
#include "../Possession/TransitionState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Curves/CurveFloat.h"
#include "../Abilities/Teleport/PestilenceTeleportAbility.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Interaction/InventoryComponent.h"

// -----------------------------------------

UFailStateComponent::UFailStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// -----------------------------------------

bool UFailStateComponent::PlayerDieResponse(const AActor* TheKillerActor)
{
	if (bIsDead) return false;

	auto* Player = Cast<APestilenceCharacter>(GetOwner());
  UPestilencePossessionComponent* PlayerPossession = Player->GetPestilencePossessionComponent();
  UPestilenceAbilityComponent* AbilityComponent = Player->GetPestilenceAbilityComponent();
	UPestilenceTeleportAbility* TeleportAbility = nullptr;
	if (AbilityComponent && AbilityComponent->GetCurrentAbility())
		TeleportAbility = Cast<UPestilenceTeleportAbility>(AbilityComponent->GetCurrentAbility());

  // Perdoname Alex, soy un vago.
	if (Player && (!PlayerPossession->IsPossessed() || PlayerPossession->GetCurrentState().IsA(UTransitionState::StaticClass())) || (TeleportAbility && TeleportAbility->IsTeleporting())) return false;

  bIsDead = true;
	OnFailStart.Broadcast();

	if (FailSound)
		UGameplayStatics::PlaySound2D(GetOwner(), FailSound);

	if (auto* PController = GetCurrentPlayerController())
		GetOwner()->DisableInput(PController);
	
	// Disable movement
	if (Player)
  {

    Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (auto PlayerMovement = Player->GetPestilenceMovementComponent())
    {
      PlayerMovement->CancelTranslations();
      PlayerMovement->StopMovementImmediately();
      PlayerMovement->EnableLook(false);
      PlayerMovement->EnableMovement(false);
      PlayerMovement->GravityScale = 0.f;
    }

    CameraComponent = Player->GetFirstPersonCameraComponent();
    if (CameraComponent)
      CameraComponent->bUsePawnControlRotation = false;

    if (auto PlayerAbilities = Player->GetPestilenceAbilityComponent())
    {
			PlayerAbilities->EnableAbilities(false);
    }

    if (auto InventoryComponent = Player->GetInventoryComponent())
    {
      InventoryComponent->DropPickable();
    }
	}

  KillerActor = TheKillerActor; 
	auto* PestilenceKiller = Cast<APestilenceCharacter>(KillerActor);
	if (ensureMsgf(PestilenceKiller, TEXT("Killer Actor is not a Pestilence Character")))
		PestilenceKiller->OnKillPlayer.Broadcast(Player->GetPestilenceMovementComponent()->IsHidden());

	UpdateAllBlackboards();
	StartDeathFall();
	
	return true;
}

// -----------------------------------------

void UFailStateComponent::Reset()
{
	bIsDead = false;
	KillerActor = nullptr;
	SetComponentTickEnabled(true);
	CurrentState = ECurrentState::None;
}

void UFailStateComponent::UpdateAllBlackboards()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),  APestilenceCharacter::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		const TObjectPtr<APestilenceCharacter> PestilenceCharacter = Cast<APestilenceCharacter>(FoundActor);
		if (!IsValid(PestilenceCharacter))
			return;
		
		TObjectPtr<APestilenceAIController> AIController = PestilenceCharacter->GetControllerAI();
		if(!IsValid(AIController))
			return;

		TObjectPtr<UBlackboardComponent> BlackboardComponent = AIController->GetBlackboard();
		if(!IsValid(BlackboardComponent))
			return;

		BlackboardComponent->SetValueAsBool(Keys::PlayerKilled, true);
	}
}

// -----------------------------------------

void UFailStateComponent::StartFallToDeath()
{
  CurrentState = ECurrentState::FallToDeath;
	OnStartDeathFall.Broadcast();
  SelectCurves();
	StartTransform = GetOwner()->GetTransform();
  StartRotEuler = GetOwner()->GetActorRotation().Euler();
  DelayTimer = 0.f;
	SetPostProcessTimer = 0.f;
	bSetPostProcess = true;
	bSetSlowdown= true;

}

// -----------------------------------------

APlayerController* UFailStateComponent::GetCurrentPlayerController()
{
	AController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!Controller)
		return nullptr;
	return Cast<APlayerController>(Controller);
}

// -----------------------------------------

void UFailStateComponent::StartDeathFall()
{
	CurrentState = ECurrentState::LookAtGuard;
	if (!KillerActor) return;
	if (auto* Player = Cast<APestilenceCharacter>(GetOwner()))
		Player->GetPestilenceMovementComponent()->StartLookAt(KillerActor->GetActorLocation(), LookAtGuardTime);
}

// -----------------------------------------

void UFailStateComponent::StartOnFloorState()
{

}

// -----------------------------------------

void UFailStateComponent::BeginPlay()
{
  Super::BeginPlay();

  float MinCurveTime, MaxCurveTime;
  if (ensureMsgf(SlowdownCurve, TEXT("Fall Rot Curve is not assigned")))
  {
		SlowdownCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    SlowdownValue = MaxCurveTime - MinCurveTime;
  }
}

// -----------------------------------------

void UFailStateComponent::Activate(const bool bReset)
{
	Super::Activate(bReset);
	//SetComponentTickEnabled(true);
}

// -----------------------------------------

void UFailStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// -----------------------------------------

void UFailStateComponent::Deactivate()
{
	Super::Deactivate();
	//SetComponentTickEnabled(false);
}

// -----------------------------------------

void UFailStateComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (CurrentState)
	{
	case ECurrentState::None:
		break;
	
	case ECurrentState::LookAtGuard:
	{
		if (!KillerActor || !CameraComponent)
		{
			CurrentState = ECurrentState::None;
			break;
		}
	}
	break;

	case ECurrentState::Delay:
	{
		DelayTimer += DeltaTime;
		if (DelayTimer >= DelayValue)
		{
      DelayTimer = 0;
      StartTransform = GetOwner()->GetTransform();
      StartRotEuler = GetOwner()->GetActorRotation().Euler();
			CurrentState = StateAfterDelay;
		}
	}
	break;

  case ECurrentState::FallToDeath:
  {
    DelayTimer += DeltaTime;
    DelayTimer = DelayTimer >= FallValue ? FallValue : DelayTimer;

    FVector CurvePosValue = FallPosCurve->GetVectorValue(DelayTimer);
		CurvePosValue.X = bIgnorePosX ? 0 : CurvePosValue.X;
		CurvePosValue.Y = bIgnorePosY ? 0 : CurvePosValue.Y;
    GetOwner()->SetActorLocation(UKismetMathLibrary::TransformLocation(StartTransform, CurvePosValue));

    FRotator NewRot = FRotator::MakeFromEuler(StartRotEuler + FallRotCurve->GetVectorValue(DelayTimer));
    GetOwner()->SetActorRotation(NewRot);

    if (DelayTimer >= FallValue)
    {
      DelayTimer = 0.f;
      CurrentState = ECurrentState::Delay;

      // Delay setup
      DelayValue = FallDelay;
      StateAfterDelay = ECurrentState::OnFloor;
    }
  }
  break;
  case ECurrentState::OnFloor:
  {
    DelayTimer += DeltaTime;
    DelayTimer = DelayTimer >= FloorValue ? FloorValue : DelayTimer;

    FRotator NewRot = FRotator::MakeFromEuler(StartRotEuler + FloorRotCurve->GetVectorValue(DelayTimer));
    GetOwner()->SetActorRotation(NewRot);

    if (DelayTimer >= FloorValue)
    {
      DelayTimer = 0.f;
      CurrentState = ECurrentState::Delay;

      // Delay setup
      DelayValue = FloorDelay;
      StateAfterDelay = ECurrentState::LoadMenu;
    }
  }
  break;
	case ECurrentState::LoadMenu:
	{
		//load menu stuff
		SetComponentTickEnabled(false);
		if (auto* Player = Cast<APestilenceCharacter>(GetOwner()))
			Player->GetPauseComponent()->AddGameOverMenu();
		if (auto* PController = GetCurrentPlayerController())
			GetOwner()->EnableInput(PController);
		CurrentState = ECurrentState::None;
	}
	break;
	// End of switch
	}

  if (bSetPostProcess)
  {
    SetPostProcessTimer += DeltaTime;
    SetPostProcessTimer = SetPostProcessTimer >= SetPostProcessTime ? SetPostProcessTime : SetPostProcessTimer;

    UMaterialParameterCollectionInstance* DeadStateParameterInstance = GetWorld()->GetParameterCollectionInstance(DeadStateParameterCollection);

    DeadStateParameterInstance->SetScalarParameterValue(OffsetName, (SetPostProcessTimer / SetPostProcessTime) * MaxPostProcessOffset);

    if (SetPostProcessTimer >= SetPostProcessTime)
      bSetPostProcess = false;
  }
  if (bSetSlowdown)
  {
    SlowdownTimer += DeltaTime;
		SlowdownTimer = SlowdownTimer >= SlowdownValue ? SlowdownValue : SlowdownTimer;

    float CurveValue = SlowdownCurve->GetFloatValue(SlowdownTimer);
		UGameplayStatics::SetGlobalTimeDilation(this, CurveValue);
    if (SlowdownTimer >= SlowdownValue)
      bSetPostProcess = false;
  }
	if(bIsDead && CurrentState != ECurrentState::LoadMenu && CurrentState != ECurrentState::None)
	{
		LoadMenuFailsafeTimer += DeltaTime;
		if (LoadMenuFailsafeTimer >= LoadMenuFailsafeTime)
			CurrentState = ECurrentState::LoadMenu;
	}
}

// -----------------------------------------

void UFailStateComponent::SelectCurves()
{
	APestilenceCharacter* Character = Cast<APestilenceCharacter>(GetOwner());
  if (!ensureMsgf(Character, TEXT("Owner is not a APestilenceCharacter"))) return;
  UPestilenceMovementComponent* MovementComponent = Character->GetPestilenceMovementComponent();
  if (!ensureMsgf(MovementComponent, TEXT("Owner has no PestilenceMovementComponent"))) return;
	if(MovementComponent->IsCrouching() || MovementComponent->IsHidden())
	{
		FallPosCurve = CrouchedFallPosCurve;
		FallRotCurve = CrouchedFallRotCurve;
		FloorRotCurve = CrouchedFloorRotCurve;
	}
	else
  {
    FallPosCurve = StandingFallPosCurve;
    FallRotCurve = StandingFallRotCurve;
    FloorRotCurve = StandingFloorRotCurve;
  }
  float MinCurveTime, MaxCurveTime;
  float MinCurveValue, MaxCurveValue;
  if (ensureMsgf(FallPosCurve, TEXT("Fall Pos Curve is not assigned")))
  {
    FallPosCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    FallValue = MaxCurveTime - MinCurveTime;

    FallPosCurve->FloatCurves[0].GetValueRange(MinCurveValue, MaxCurveValue);
    FHitResult Hit;

		// Repasar esto que es feisimo
    FVector Start = FVector(0 < MaxCurveValue ? MaxCurveValue + Character->GetCapsuleComponent()->GetScaledCapsuleRadius() : 0, 0, 0);
    Start = UKismetMathLibrary::TransformLocation(GetOwner()->GetTransform(), Start);
    FVector End = FVector(MinCurveValue < 0 ? MinCurveValue - Character->GetCapsuleComponent()->GetScaledCapsuleRadius() : 0, 0, 0);
    End = UKismetMathLibrary::TransformLocation(GetOwner()->GetTransform(), End);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    bIgnorePosX = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);
    FallPosCurve->FloatCurves[1].GetValueRange(MinCurveValue, MaxCurveValue);

    Start = FVector(0, 0 < MaxCurveValue ? MaxCurveValue + Character->GetCapsuleComponent()->GetScaledCapsuleRadius() : 0, 0);
    Start = UKismetMathLibrary::TransformLocation(GetOwner()->GetTransform(), Start);
    End = FVector(0, MinCurveValue < 0 ? MinCurveValue - Character->GetCapsuleComponent()->GetScaledCapsuleRadius() : 0, 0);
    End = UKismetMathLibrary::TransformLocation(GetOwner()->GetTransform(), End);
    
    bIgnorePosY = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams);
  }
  if (ensureMsgf(FallRotCurve, TEXT("Fall Rot Curve is not assigned")))
  {
    FallRotCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    ensureMsgf(FallValue == MaxCurveTime - MinCurveTime, TEXT("Fall Curves have different times"));
  }
  if (ensureMsgf(FloorRotCurve, TEXT("Fall Rot Curve is not assigned")))
  {
    FloorRotCurve->GetTimeRange(MinCurveTime, MaxCurveTime);
    FloorValue = MaxCurveTime - MinCurveTime;
  }
}

// -----------------------------------------