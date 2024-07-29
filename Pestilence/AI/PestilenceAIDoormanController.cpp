// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceAIDoormanController.h"

#include "AIHelpers.h"
#include "Behaviors/blackboard_keys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/Animation/PestilenceCharacter/PestilenceCharacterAnimInstance.h"
#include "Pestilence/Components/AI/PestilenceAIComponent.h"
#include "Pestilence/Components/Perception/PestilenceSightComponent.h"
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Pestilence/Props/ArchetypeZone.h"
#include "Pestilence/Props/DoormanRumourZone.h"
#include "Pestilence/UI/PestilenceConversationUserWidget.h"

APestilenceAIDoormanController::APestilenceAIDoormanController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ControllerType = EPestilenceControllerType::EDoormanController;
}

void APestilenceAIDoormanController::BeginPlay()
{
	Super::BeginPlay();
}

void APestilenceAIDoormanController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsValid(GetPawn())) return;

	const TObjectPtr<AActor> PlayerCharacter = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if(!IsValid(PlayerCharacter)) return;
	const float DistanceToPlayer = FVector::DistSquared(WarningTargetPoint->GetActorLocation(),PlayerCharacter->GetActorLocation());
	if(!IsValid(GetBlackboard())) return;
	EPestilenceStates State = static_cast<EPestilenceStates>(GetBlackboard()->GetValueAsEnum(Keys::State));
	TObjectPtr<UPestilenceSightComponent> SightComponent = FindComponentByClass<UPestilenceSightComponent>();

	if(State == EPestilenceStates::Doorman)
	{
		//Is in Warning Point
		if(GetBlackboard()->GetValueAsBool(Keys::MoveToWarningTargetPoint))
		{
			if(	DistanceToPlayer > (WarningDistance * WarningDistance) || !AssignedZone->ActorIsRestricted(PlayerCharacter))
			{
				bHasWarned = false;
				//RumourZone->bIsPlayerInZone = false;
				bCorrectConversationPlayed = false;
				GetBlackboard()->SetValueAsBool(Keys::MoveToWarningTargetPoint,false);
			}
			else
			{
				if(bIsInMyZone && bHasWarned)
					if(SightComponent->bOnSight || SightComponent->IsOnCloseConeOfVision)
						Attention = MaxAttention;
			}
		}
		else
		{
			Cast<UPestilenceCharacterAnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance())->bIsInRumour = false;
			Cast<APestilenceCharacter>(GetCharacter())->OnTalkRumour.Broadcast(false);
			
			if( (SightComponent->bOnSight || SightComponent->IsOnCloseConeOfVision) && DistanceToPlayer < (WarningDistance * WarningDistance))
			{
				if(AssignedZone->ActorIsRestricted(PlayerCharacter) && !bHasWarned)
				{
					bHasWarned = true;
					RumourZone->bIsPlayerInZone = true;
					RumourZone->InCorrectConversation(RumourZone->CanNotEnterConversationAsset);
					GetBlackboard()->SetValueAsBool(Keys::MoveToWarningTargetPoint,true);
				}
				else
				{
					RumourZone->bIsPlayerInZone = true;
					if(!bCorrectConversationPlayed)
					{
						RumourZone->CorrectConversation(RumourZone->CanEnterConversationAsset);
						bCorrectConversationPlayed = true;
					}
				}
			}
			else
			{
				bHasWarned = false;
				bCorrectConversationPlayed = false;
			}
		}
	}
	else
	{
		bHasWarned = false;
		bCorrectConversationPlayed = false;
		GetBlackboard()->SetValueAsBool(Keys::MoveToWarningTargetPoint,false);
	}
}

void APestilenceAIDoormanController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(!IsValid(GetPawn()) || !IsValid(GetBlackboard())) return;
	if(!IsValid(GetPawn()->FindComponentByClass<UPestilenceAIComponent>())) return;
	const TObjectPtr<UPestilenceAIComponent> AIComponent = GetPawn()->FindComponentByClass<UPestilenceAIComponent>();
	AssignedZone = AIComponent->AssignedZone;
	WarningTargetPoint = AIComponent->WarningTargetPoint;
	ensureMsgf(AssignedZone,TEXT("A door guard always needs an archetype zone"));
	
	AssignedZone->OnActorBeginOverlap.AddDynamic(this,&APestilenceAIDoormanController::OnPlayerEnter);
	AssignedZone->OnActorEndOverlap.AddDynamic(this,&APestilenceAIDoormanController::OnPlayerExit);

	RumourZone = Cast<ADoormanRumourZone>(PestilenceAIComponent->RumourZone);
	ensureMsgf(RumourZone,TEXT("A door guard always needs an rumour zone to tell rumours"));

	TObjectPtr<UPestilencePossessionComponent> PossessionComponent = GetPawn()->FindComponentByClass<UPestilencePossessionComponent>();
	if(PossessionComponent)
		PossessionComponent->OnEndPossessEvent().AddDynamic(this,&APestilenceAIDoormanController::ControlPossesions);

	if(IsValid(WarningTargetPoint))
	{
		GetBlackboard()->SetValueAsVector(Keys::WarningTargetPointPosition,WarningTargetPoint->GetActorLocation());
		GetBlackboard()->SetValueAsVector(Keys::WarningTargetPointRotation,WarningTargetPoint->Pivot->GetComponentLocation());
	}
}

void APestilenceAIDoormanController::OnPlayerEnter(AActor* OverlappedActor, AActor* OtherActor)
{
	const TObjectPtr<AActor> Player = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if(!IsValid(Player)) return;
	if(OtherActor != Player) return;

	if(AssignedZone->ActorIsRestricted(OtherActor))
		bIsInMyZone = true;

	TObjectPtr<UPestilenceSightComponent> SightComponent = FindComponentByClass<UPestilenceSightComponent>();
	if(!IsValid(SightComponent)) return;
	SightComponent->ControlActorsOnSight(true,OtherActor);
	SightComponent->ControlSuspiciosActionSubscription(OtherActor);
}

void APestilenceAIDoormanController::OnPlayerExit(AActor* OverlappedActor, AActor* OtherActor)
{
	bIsInMyZone = false;
	
	TObjectPtr<UPestilenceSightComponent> SightComponent = FindComponentByClass<UPestilenceSightComponent>();
	if(!IsValid(SightComponent)) return;
	SightComponent->ControlActorsOnSight(false,OtherActor);
	SightComponent->ControlSuspiciosActionSubscription(OtherActor);

}

void APestilenceAIDoormanController::ControlPossesions()
{
	if(GetCurrentState()== EPestilenceStates::Dead) return;
	AssignedZone->Refresh();
	const TObjectPtr<AActor> Player = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	if(bIsInForbiddenZone)
	{
		OnPlayerEnter(AssignedZone,Player);
	}
	else
	{
		OnPlayerExit(AssignedZone,Player);
	}

	bIsInMyZone = bIsInMyZone;
}
