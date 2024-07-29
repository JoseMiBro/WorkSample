#include "PossessedState.h"
#include "PestilencePossessionComponent.h"
#include "PestilenceVisionComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "TransitionState.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/UI/PlayerHUDWidget.h"
#include "Camera/CameraComponent.h"

void UPossessedState::CreateState()
{
	const auto Possession = GetPossessionComponent();
	if (!Possession) return;
	CurrentIntegrity = Possession->MaxIntegrity;

	Owner = Possession->GetOwner<APestilenceCharacter>();
	VisionComponent = Owner->GetPestilenceVisionComponent();
	PlayerCameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APestilenceAIController::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		AIControllerActors.Push(Cast<APestilenceAIController>(Actor));
	}

	const auto HandsMat = Owner->GetMesh1P()->GetMaterial(0);
	if (HandsMat)
	{
		DynamicHandsMaterial = UMaterialInstanceDynamic::Create(HandsMat, this);
		if (DynamicHandsMaterial)
			Owner->GetMesh1P()->SetMaterial(0, DynamicHandsMaterial);
	}
}

void UPossessedState::EnterState()
{
	Super::EnterState();

	ResetValues();
	// cambiar por npc manager
	for (APestilenceAIController* Controller : AIControllerActors)
	{
		if (!IsValid(Controller)) continue;
		Controller->SetIntegrityFactor(GetPossessionComponent()->MaxIntegrity);
	}

	GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceFOV"), DefaultFov, GGameIni);
	Owner->EnableInput(Cast<APlayerController>(Owner->GetController()));

	auto MovementComponent = Owner->GetPestilenceMovementComponent();
	if (GetPossessionComponent()->bCanMove && MovementComponent)
	{
		MovementComponent->EnableMovement(true);
		MovementComponent->EnableLook(true);
	}

	VisionInputEventCallback = GetPossessionComponent()->OnVisionInputEvent().AddLambda([&](const bool bPressed)
		{
			VisionComponent->SetActive(bPressed);
		});

	PossessionInputEventCallback = GetPossessionComponent()->OnPossessionInputEvent().AddLambda([&](const bool bPressed)
		{
			LookingCharacter = VisionComponent->GetLookingCharacter();

	if (!LookingCharacter) return;

	if (bPressed && VisionComponent->IsActive() && GetPossessionComponent()->bDisableInputOnPossessing)
	{
		Owner->GetPestilenceMovementComponent()->EnableMovement(false);
		Owner->GetPestilenceMovementComponent()->EnableLook(false);
	}

	bTryingToPossessLookingCharacter = bPressed;
	PossessionAttemptTime = 0.f;

	if (!bPressed && GetPossessionComponent()->bDisableInputOnPossessing)
	{
		PlayerCameraManager->SetFOV(DefaultFov);
		Owner->GetPestilenceMovementComponent()->EnableMovement(true);
		Owner->GetPestilenceMovementComponent()->EnableLook(true);
	}
		});

  PlayerCameraManager->SetFOV(DefaultFov);
	GetPossessionComponent()->SetPossessed(true);
	GetPossessionComponent()->OnPossessed.Broadcast();
}

void UPossessedState::UpdateState(const float DeltaTime)
{
	Super::UpdateState(DeltaTime);

	if (GetPossessionComponent()->bDegradationEnabled)
	{
		const float PrevIntegrity = CurrentIntegrity;
		if (CurrentIntegrity > 0)
		{
			CurrentIntegrity -= GetPossessionComponent()->DegradationSpeed * DeltaTime;
			CurrentIntegrity = FMath::Clamp(CurrentIntegrity, 0, GetPossessionComponent()->MaxIntegrity);
			if (const TObjectPtr<UPlayerHUDWidget> HUDWidget = Owner->GetPlayerHUD())
				HUDWidget->SetIntegrityBar(CurrentIntegrity, GetPossessionComponent()->MaxIntegrity);
		}

		for (APestilenceAIController* Controller : AIControllerActors)
		{
			if (!Controller) continue;
			Controller->SetActualIntegrity(CurrentIntegrity);
		}

		if (IsValid(DynamicHandsMaterial))
			DynamicHandsMaterial->SetScalarParameterValue("Blend", (1 - CurrentIntegrity / GetPossessionComponent()->MaxIntegrity) - 0.5f);

		GetPossessionComponent()->IntegrityChangedDelegate.Broadcast(PrevIntegrity, CurrentIntegrity);
	}

	if (!bTryingToPossessLookingCharacter) return;

	if (!LookingCharacter)
	{
		ResetValues();
		return;
	}

	//TODO: Camera fov
	if (LookingCharacter)
		LookingCharacter->SetOverlayMaterialUsing();

	PossessionAttemptTime += DeltaTime;
	PlayerCameraManager->SetFOV(PlayerCameraManager->GetFOVAngle() - GetPossessionComponent()->GetFovSpeedOnPossess() * DeltaTime);

	FVector Direction = LookingCharacter->GetActorLocation() - GetPossessionComponent()->GetPestilenceOwner()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FRotator NewRot = Direction.Rotation();
	if (GetPossessionComponent()->GetPestilenceOwner()->GetController())
		GetPossessionComponent()->GetPestilenceOwner()->GetController()->SetControlRotation(NewRot);
	else
		GetPossessionComponent()->GetOwner()->SetActorRotation(NewRot);

	if (PossessionAttemptTime >= GetPossessionComponent()->GetTimeToPossess())
		JumpToTransitionState();
}

void UPossessedState::JumpToTransitionState()
{
	const auto TransitionState = GetPossessionComponent()->GetTransitionState();
	TransitionState->SetPossessingCharacter(LookingCharacter);
	GetPossessionComponent()->SwitchState(TransitionState);
}

void UPossessedState::ResetValues()
{
	bTryingToPossessLookingCharacter = false;
	VisionComponent->SetActive(false);

	PlayerCameraManager->SetFOV(DefaultFov);

	if (!GetPossessionComponent()->bDisableInputOnPossessing) return;

	Owner->GetPestilenceMovementComponent()->EnableMovement(true);
	Owner->GetPestilenceMovementComponent()->EnableLook(true);
}

void UPossessedState::ExitState()
{
	Super::ExitState();
	CurrentIntegrity = 0;

	// cambiar a que use el npc manager
	for (APestilenceAIController* Controller : AIControllerActors)
	{
		if (!IsValid(Controller)) continue;
		Controller->SetIntegrityFactor(0.f);
	}

	GetPossessionComponent()->OnVisionInputEvent().Remove(VisionInputEventCallback);
	Owner->DisableInput(Cast<APlayerController>(Owner->GetController()));
	ResetValues();
}
