// Fill out your copyright notice in the Description page of Project Settings.
#include "PestilenceSightComponent.h"

#include "EngineUtils.h"
#include "SuspiciousActionSenderComponent.h"
#include "VectorTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Actions/PawnAction.h"
#include "AIHelpers.h"
#include "Pestilence/AI/Interface/PestilenceAIInterface.h"
#include "Pestilence/Animation/PestilenceCharacter/PestilenceCharacterAnimInstance.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/AI/AIReactionComponent.h"


// Sets default values for this component's properties
UPestilenceSightComponent::UPestilenceSightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPestilenceSightComponent::ChangeVisionConfig(EPestilenceStates State)
{
	const TObjectPtr<UAIPerceptionComponent> PerceptionComponent = OwnerController->PerceptionComponent;

	if(State == EPestilenceStates::Alert || State == EPestilenceStates::Chase)
	{
		PerceptionComponent->ConfigureSense(*WideSightConfig);
	}
	else
	{
		PerceptionComponent->ConfigureSense(*SightConfig);
	}
	PerceptionComponent->RequestStimuliListenerUpdate();
}

void UPestilenceSightComponent::BeginPlay()
{
	Super::BeginPlay();
	//Init();
}

void UPestilenceSightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!IsValid(OwnerController)) return;

	if(OwnerController->GetCurrentState() == EPestilenceStates::Dead ||
		OwnerController->GetCurrentState() == EPestilenceStates::Infected) return;
	if(!OwnerController->GetBlackboard()) return;
	const EPestilenceStates State = TEnumAsByte<EPestilenceStates>(OwnerController->GetBlackboard()->GetValueAsEnum(Keys::State));

	if((bOnSight  || IsOnCloseConeOfVision) && State != EPestilenceStates::Stun)
	{
 		CalculateVisibilityFactor();
		const float VisibilityFactor = OwnerController->GetVisibilityFactor();
		if(VisibilityFactor <= 0.f )
		{
			OwnerController->SetVisibilityFactor(0.f);
			CalculateAttentionLost(DeltaTime);
		}
		else
		{
			OwnerController->CalculateIntegrityFactor();
			if(  OwnerController->GetIntegrityFactor() == 0.f &&
				!OwnerController->GetBlackboard()->GetValueAsBool(Keys::Witness) &&
				!OwnerController->GetIsInForbiddenZone())
			{
				OwnerController->SetVisibilityFactor(0.f);
				CalculateAttentionLost(DeltaTime);
			}
			else
			{
				if(State == EPestilenceStates::Doorman && OwnerController->GetControllerType() == EPestilenceControllerType::EDoormanController)
					RotateToLookAtPlayer();
				if (State == EPestilenceStates::Advise)
					OwnerController->UpdateAdviseAttention();
				OwnerController->UpdateAttention(EAttentionUpdateType::ESightUpdate);
			}
		}
	}
	else
	{
		OwnerController->SetVisibilityFactor(0.f);
		CalculateAttentionLost(DeltaTime);
	}
	
	const TObjectPtr<APestilenceCharacter> ControlledActor = Cast<APestilenceCharacter>(OwnerController->GetPawn());
	if(!IsValid(ControlledActor)) return;
	const TObjectPtr<AActor> PlayerActor = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if(!IsValid(PlayerActor)) return;

	const FVector ControlledActorLocation = ControlledActor->GetActorLocation();
	const FVector PlayerActorLocation = PlayerActor->GetActorLocation();

	FVector LowControlledActorLocation =  ControlledActorLocation;
	LowControlledActorLocation.Z = LowControlledActorLocation.Z - CloseSightConeHeightOffset;
	FVector LowPlayerActorLocation = PlayerActorLocation ;
	LowPlayerActorLocation.Z = LowPlayerActorLocation.Z - CloseSightConeHeightOffset;
	FVector BackwardControlledActorLocation = LowControlledActorLocation;
	BackwardControlledActorLocation -= (ControlledActor->GetActorForwardVector() * BackwardConeOffset);
	
	if(bDrawDebugCloseConeOfVision)
		DrawDebugCone(
			GetWorld(),
			BackwardControlledActorLocation,
			ControlledActor->GetActorForwardVector(),
			CloseSightDistance,
			FMath::DegreesToRadians(CloseSightHalfAngle*2),//60
			FMath::DegreesToRadians(CloseSightHalfAngle*2),//60
			12,FColor::Cyan);
	
	IsOnCloseConeOfVision = FAISystem::CheckIsTargetInSightCone(
	LowControlledActorLocation,
	ControlledActor->GetActorForwardVector(),
	FMath::Cos(FMath::DegreesToRadians(CloseSightHalfAngle*2)),//60
	BackwardConeOffset,//20
	BackwardConeOffset,//20
	CloseSightDistance*CloseSightDistance + BackwardConeOffset,//20
	LowPlayerActorLocation);
	
	if(IsOnCloseConeOfVision && !bPlayerSighted)
	{
		OnPlayerSighted.Broadcast();
		bPlayerSighted = true;
	}

	if((bOnSight || IsOnCloseConeOfVision) &&((State == EPestilenceStates::Escape && !OwnerController->GetBlackboard()->GetValueAsBool(Keys::SomeOneInfected)) || State == EPestilenceStates::Chase))
	{
		if(!OwnerController->GetBlackboard()->GetValueAsBool(Keys::Witness))
			OwnerController->OnWitnessChange.Broadcast(true);
		OwnerController->GetBlackboard()->SetValueAsBool(Keys::Witness,true);
	}
}

void UPestilenceSightComponent::ProcessSightStimuli(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.Type != SightConfig->GetSenseID()) return;
	if(OwnerController->GetCurrentState() == EPestilenceStates::Dead) return;
	
	ControlActorsOnSight(Stimulus.WasSuccessfullySensed(),Actor);
	
	if(Actor == UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn())
	{
		ControlOnSight(Stimulus.WasSuccessfullySensed(),Actor);
		IsOnConeOfVision = Stimulus.WasSuccessfullySensed();
		bPlayerSighted = IsOnConeOfVision;
		if(bPlayerSighted)
			OnPlayerSighted.Broadcast();
	}
	else if (IPestilenceAIInterface* PestilenceAIInterface = Cast<IPestilenceAIInterface>(Actor))
	{
		//controller
		const TObjectPtr<APestilenceCharacter> PestilenceCharacter = Cast<APestilenceCharacter>(Actor);
		if (!IsValid(PestilenceCharacter)) return;
		const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(PestilenceCharacter->GetController());
		if (!IsValid(PestilenceAIController)) return;
		
		bSightAI = Stimulus.WasSuccessfullySensed();

		if(ReactionComponent)
			if (bSightAI)
			{
				ReactionComponent->SetReactionMap(PestilenceAIController, PestilenceAIInterface->GetTypeCharacter());
			}
			else
			{
				ReactionComponent->SetRemoveReactionMap(PestilenceAIController);
			}
	}
}

void UPestilenceSightComponent::ConfigSight() 
{
	const TObjectPtr<UAIPerceptionComponent> PerceptionComponent = OwnerController->PerceptionComponent;
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->RequestStimuliListenerUpdate();
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&UPestilenceSightComponent::ProcessSightStimuli);
}

void UPestilenceSightComponent::CalculateVisibilityFactor() const
{
	const TObjectPtr<APawn> PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	const TObjectPtr<APawn> OwnerPawn = OwnerController->GetPawn();

	if(!IsValid(PlayerPawn)) return;
	if(!IsValid(OwnerPawn)) return;
	
	const TObjectPtr<UPestilenceMovementComponent> MovementComponent = PlayerPawn->FindComponentByClass<UPestilenceMovementComponent>();
	if(!IsValid(MovementComponent)) return;
	
	if (SightCalculator)
	{
		SightCalculator->PlayerPawn = PlayerPawn;
		SightCalculator->OwnerPawn = OwnerPawn;
		SightCalculator->MovementComponent = MovementComponent;
		SightCalculator->bOnShadow = this->bOnShadow;
		SightCalculator->OwnerController = this->OwnerController;
		SightCalculator->SightConfig = this->SightConfig;
		SightCalculator->IsOnCloseConeOfVision = this->IsOnCloseConeOfVision;
		OwnerController->SetVisibilityFactor(ICalculator::Execute_CalculateAttention(SightCalculator));
	}
}

void UPestilenceSightComponent::SetOnShadow(bool IsOnShadow)
{
	bOnShadow = IsOnShadow;
	OnShadowChanged.Broadcast(bOnShadow);
}

void UPestilenceSightComponent::Init()
{
	OwnerController = Cast<APestilenceAIController>(GetOwner());

	ensureMsgf(OwnerController,TEXT("Sight Components require of an PestilenceAIController"));
	
	ConfigSight();
	
	OwnerCharacter = Cast<APestilenceCharacter>(OwnerController->GetPawn());
	if(IsValid(OwnerCharacter))
	{
		const TObjectPtr<UPestilencePossessionComponent> PossessionComponent = OwnerController->GetPawn()->FindComponentByClass<UPestilencePossessionComponent>();
		OwnerType = OwnerCharacter->PestilenceType;
		ReactionComponent = OwnerCharacter->FindComponentByClass<UAIReactionComponent>();
		CharacterAnimationInstance = Cast<UPestilenceCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
		if(!PossessionComponent->OnPossessed.IsAlreadyBound(this,&UPestilenceSightComponent::CheckOnSight))
			PossessionComponent->OnPossessed.AddDynamic(this, &UPestilenceSightComponent::CheckOnSight);
	}
}

void UPestilenceSightComponent::CalculateAttentionLost(float DeltaTime)
{
	EPestilenceStates State = EPestilenceStates::None;
	const TObjectPtr<UBlackboardComponent> BlackboardComponent = OwnerController->GetBlackboard();

	if(IsValid(BlackboardComponent))
		State = static_cast<EPestilenceStates>(OwnerController->GetBlackboard()->GetValueAsEnum(Keys::State));

	float AttentionToRemove = 0.f;
	switch (State)
	{
		case EPestilenceStates::Patrol:
			AttentionToRemove = PatrolAttentionLose;break;
		case EPestilenceStates::Alert:
			AttentionToRemove = AlertAttentionLose;break;
		case EPestilenceStates::Chase:
			AttentionToRemove = ChaseAttentionLose;break;
		default:
			AttentionToRemove = StandarAttentionLose;
	}
	OwnerController->RemoveAttention(AttentionToRemove * DeltaTime);
}

void UPestilenceSightComponent::SuspiciousActionReceived(const FSuspiciousAction& SuspiciousAction)
{
	if(OwnerController->GetCurrentState() == EPestilenceStates::Dead) return;
	OwnerController->SetSuspiciousActionFactor(SuspiciousAction.Points);
	OwnerController->UpdateAttention(EAttentionUpdateType::ESuspiciousActionUpdate);

	OnSuspiciousActionReceived.Broadcast(SuspiciousAction);
}

void UPestilenceSightComponent::ControlOnSight(bool IsOnSight, AActor* Actor)
{
	bOnSight = IsOnSight;
	if (IsValid(OwnerController->GetBlackboard()))
  {
    bool bSeeingHiding = OwnerController->GetBlackboard()->GetValueAsBool(Keys::SeeingHiding);
    bool bPlayerHidden = OwnerController->GetBlackboard()->GetValueAsBool(Keys::PlayerHidden);
    bOnSight = bOnSight && (!bPlayerHidden || bSeeingHiding);
		OwnerController->GetBlackboard()->SetValueAsBool(Keys::SeeingPlayer, bOnSight);
  }
  ControlSuspiciosActionSubscription(Actor);
}

void UPestilenceSightComponent::ControlActorsOnSight(bool Add, AActor* Actor)
{
	TObjectPtr<UPestilencePossessionComponent> PossessionComponent = Actor->FindComponentByClass<UPestilencePossessionComponent>();
	if(!IsValid(PossessionComponent)) return;
	
	if(Add)
	{
		ActorsOnSight.Add(Actor);
	}
	else
	{
		ActorsOnSight.Remove(Actor);
	}
}
void UPestilenceSightComponent::CheckOnSight()
{
	if(OwnerController->GetCurrentState() == EPestilenceStates::Dead) return;
	const TObjectPtr<AActor> PlayerPawn = OwnerController->GetPawn();
	if(IsValid(OwnerController->GetBlackboard()))
	{
		for(TActorIterator<APestilenceAIController> ControllerItr(GetWorld());ControllerItr;++ControllerItr)
		{
			const TObjectPtr<APestilenceAIController> ControllerToCheck = *ControllerItr;
			TObjectPtr<UPestilenceSightComponent> SightComponent = ControllerToCheck->FindComponentByClass<UPestilenceSightComponent>();
			if(SightComponent->ActorsOnSight.Num() > 0 && SightComponent->ActorsOnSight.Contains(PlayerPawn))
			{
				SightComponent->bOnSight = true;
			}
			else
			{
				SightComponent->bOnSight = false;
			}

			if (IsValid(ControllerItr->GetBlackboard()))
			{
				bool bSeeingHiding = ControllerToCheck->GetBlackboard()->GetValueAsBool(Keys::SeeingHiding);
				bool bPlayerHidden = ControllerToCheck->GetBlackboard()->GetValueAsBool(Keys::PlayerHidden);
				SightComponent->bOnSight = SightComponent->bOnSight && (!bPlayerHidden || bSeeingHiding);
				ControllerToCheck->GetBlackboard()->SetValueAsBool(Keys::SeeingPlayer, SightComponent->bOnSight);
			}
		}
	}
}

void UPestilenceSightComponent::ControlSuspiciosActionSubscription(const AActor* Actor)
{
	const TObjectPtr<USuspiciousActionSenderComponent> SuspiciousActionSenderComponent = Actor->FindComponentByClass<USuspiciousActionSenderComponent>();
	if(!IsValid(SuspiciousActionSenderComponent)) return;
	FSuspiciousActionPerformed& SuspiciousActionPerformed = SuspiciousActionSenderComponent->OnSuspiciousActionPerformed();
	
	if(!IsValid(OwnerController->GetBlackboard())) return;
	const EPestilenceStates State = TEnumAsByte<EPestilenceStates>(OwnerController->GetBlackboard()->GetValueAsEnum(Keys::State));

	if(bOnSight && State != EPestilenceStates::Stun)
	{
		if(!SuspiciousActionPerformed.IsAlreadyBound(this, &UPestilenceSightComponent::SuspiciousActionReceived))
			SuspiciousActionPerformed.AddDynamic(this, &UPestilenceSightComponent::SuspiciousActionReceived);
	}
	else
    {
		if(SuspiciousActionPerformed.IsAlreadyBound(this, &UPestilenceSightComponent::SuspiciousActionReceived))
			SuspiciousActionPerformed.RemoveDynamic(this, &UPestilenceSightComponent::SuspiciousActionReceived);
	}
}

void UPestilenceSightComponent::ControlSightOnPossesion()
{
	const TObjectPtr<AActor> PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	
	ControlSuspiciosActionSubscription(PlayerPawn);
	if(IsValid(OwnerController->GetBlackboard()))
		CheckOnSight();

}

void UPestilenceSightComponent::RotateToLookAtPlayer() const
{
	if(OwnerController->GetCurrentState() == EPestilenceStates::Dead) return;
	const TObjectPtr<AActor> PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	const TObjectPtr<AActor> OwnerPawn = OwnerController->GetPawn();
	if(!IsValid(PlayerPawn) || !IsValid(OwnerPawn)) return;
	
	const FVector PlayerLoc = PlayerPawn->GetActorLocation();
	const FVector OwnerLoc = OwnerPawn->GetActorLocation();

	FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(
		OwnerLoc,
		PlayerLoc
		);
	
	PlayerRot.Pitch = PlayerRot.Roll = 0.f;

	const EPestilenceStates State = TEnumAsByte<EPestilenceStates>(OwnerController->GetBlackboard()->GetValueAsEnum(Keys::State));
	
	if (State == EPestilenceStates::Patrol ||
		State == EPestilenceStates::Alert ||
		State == EPestilenceStates::Doorman ||
		State == EPestilenceStates::Chase )
	{
		 FRotator NewRot = FMath::RInterpTo(
			OwnerPawn->GetActorRotation(),
			PlayerRot,
			GetWorld()->GetDeltaSeconds(),
			RotationSpeed
			);
		NewRot.Pitch = 0;
		OwnerPawn->SetActorRotation(NewRot);
	}
}
