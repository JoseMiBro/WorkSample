#include "TransitionState.h"
#include "PestilencePossessionComponent.h"
#include "Camera/CameraComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/Possession/PossessedState.h"
#include "Pestilence/Components/Possession/FreeState.h"
#include "Pestilence/Components/Possession/RagdollState.h"
#include "Pestilence/Core/PestilenceGameInstance.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"

//TODO: Make camera look target smoothly
//TODO: Apply fade when camera arrives to the character

void UTransitionState::CreateState()
{
	const auto Possession = GetPossessionComponent();
	if (!Possession) return;
	Owner = Possession->GetOwner<APestilenceCharacter>();
	CameraComponent = Owner->GetFirstPersonCameraComponent();
}

void UTransitionState::EnterState()
{
	Super::EnterState();
	StartSelfCameraLocation = CameraComponent->GetComponentLocation();
	StartSelfCameraRotation = CameraComponent->GetComponentRotation();
	GetPossessionComponent()->OnBeginPossessEvent().Broadcast();
}

void UTransitionState::UpdateState(const float DeltaTime)
{	
	Super::UpdateState(DeltaTime);
	
	const FVector SelfCameraLocation = CameraComponent->GetComponentLocation();
	const FVector TargetCameraLocation = PossessingCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector NewSelfCameraLocation = FMath::Lerp(SelfCameraLocation, TargetCameraLocation, GetPossessionComponent()->GetTransitionSmoothing());
	
	const float DistanceToTarget = FVector::Distance(NewSelfCameraLocation, TargetCameraLocation);
	
	static constexpr float ReachDistance = 120.f;
	static constexpr float FadeTime = .5f;
	
	if (DistanceToTarget <= ReachDistance)
	{
		GetPossessionComponent()->CloseToTargetDelegate.Broadcast();
		TransitionTime += DeltaTime;
	}
	
	if (TransitionTime >= FadeTime)
	{
		TransitionTime = 0;
		CameraComponent->SetWorldLocation(NewSelfCameraLocation);
		GetPossessionComponent()->TargetReachedDelegate.Broadcast();
		CameraComponent->SetWorldLocation(TargetCameraLocation);
		const auto OtherPossessionComponent = PossessingCharacter->GetPestilencePossessionComponent();
		OtherPossessionComponent->SwitchState(OtherPossessionComponent->GetPossessedState());
		Cast<APlayerController>(Owner->GetController())->Possess(PossessingCharacter);
		GetPossessionComponent()->OnEndPossessEvent().Broadcast();
		GetPossessionComponent()->SwitchState(GetPossessionComponent()->GetRagdollState());
	}
	
	CameraComponent->SetWorldLocation(NewSelfCameraLocation);
}

void UTransitionState::ExitState()
{
	Super::ExitState();
	CameraComponent->SetWorldLocation(StartSelfCameraLocation);
	GetPossessionComponent()->SetPossessed(false);

	UPestilenceGameInstance* GameInstance = GetWorld()->GetGameInstance<UPestilenceGameInstance>();
	if (GameInstance)
	{
		GameInstance->RemoveLoadingScreen();
		if (PossessingCharacter)
		{
			//PossessingCharacter->GetPestilenceMovementComponent()->EnableMovement(true); // ???????????
			PossessingCharacter->GetPestilenceMovementComponent()->MaxWalkSpeed = 600.f;
		}
	}
}