// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceHearingComponent.h"

#include "PestilenceSightComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Components/AI/AIReactionComponent.h"
#include "Pestilence/Components/Interaction/InteractableComponent.h"
#include "Pestilence/Components/Possession/ArchetypeComponent.h"

// Sets default values for this component's properties
UPestilenceHearingComponent::UPestilenceHearingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPestilenceHearingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerController = Cast<APestilenceAIController>(GetOwner());
	
	ensureMsgf(IsValid(OwnerController),TEXT("Hearing Components require of an PestilenceAIController"));

	if(IsValid(OwnerController->GetBlackboard()))
		OwnerController->GetBlackboard()->SetValueAsFloat(Keys::LoseInterestWaitTime,LoseInteresetTime);
	
	ConfigHearing();
}


// Called every frame
void UPestilenceHearingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if(OwnerController->GetCurrentState() == EPestilenceStates::Dead) return;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	EPestilenceStates CurrentState;
	const bool IsInHearingState = CheckForHearingState(CurrentState);
	TObjectPtr<UBlackboardComponent> OwnerBlackboard = OwnerController->GetBlackboard();
	if(!IsValid(OwnerBlackboard)) return;
	
	if(IsInHearingState)
	{
		if( OwnerController->GetCurrentState() == EPestilenceStates::Patrol ||
			OwnerController->GetCurrentState() == EPestilenceStates::Alert ||
			OwnerController->GetCurrentState() == EPestilenceStates::Doorman)
		{
			if (bLookAtStimuliIsValid)
			{
				if(!OwnerController->FindComponentByClass<UPestilenceSightComponent>()->bOnSight)
				{
					if(LookAtStimuliSource(LookAtStumuli.StimulusLocation))
					{
						if(!bLookAtSomething)
						{
							if(LookAtStumuli.Tag.Compare(SoundTypeToString(SoundType::BottleBraking)) == 0)
							{
								OnSoundHeard.Broadcast(SoundType::BottleBraking);
							}
							else
							{
								OnSoundHeard.Broadcast(SoundType::Step);
							}
						}
						bLookAtSomething = true;
						//OwnerController->GetBlackboard()->SetValueAsFloat(
						//Keys::HeardSomethingOnPatrolWaitTime,
						//0.f
						//);
						//OwnerController->GetBlackboard()->SetValueAsBool(Keys::HeardSomethingOnPatrol,false);
						//OwnerController->GetBlackboard()->SetValueAsBool(Keys::HeardSomethingOnPatrol,true);
						//OwnerController->GetBlackboard()->SetValueAsFloat(
						//Keys::HeardSomethingOnPatrolWaitTime,
						//LookAtTimerReset
						//);
						OwnerController->StopMovement();
					}
					else
					{

						//OwnerController->GetBlackboard()->SetValueAsFloat(Keys::HeardSomethingOnPatrolWaitTime, 0);
					}
				}
				else
				{
					bLookAtSomething = false;
					bLookAtStimuliIsValid = false;
					//OwnerController->GetBlackboard()->SetValueAsFloat(
					//Keys::HeardSomethingOnPatrolWaitTime, 0);
					OwnerController->GetBlackboard()->SetValueAsBool(Keys::HeardSomethingOnPatrol,false);
					OwnerBlackboard->SetValueAsBool(Keys::IsRotating,false);
				}

			}else
			{
				bLookAtSomething = false;
			
				bLookAtStimuliIsValid = false;
				//OwnerController->GetBlackboard()->SetValueAsBool(Keys::HeardSomethingOnPatrol,false);
				OwnerBlackboard->SetValueAsBool(Keys::IsRotating,false);
			}
		}
		else
		{
			bLookAtSomething = false;
			
			bLookAtStimuliIsValid = false;
			OwnerController->GetBlackboard()->SetValueAsBool(Keys::HeardSomethingOnPatrol,false);
			OwnerBlackboard->SetValueAsBool(Keys::IsRotating,false);

		}
	}
	else
	{
		bLookAtSomething = false;
		//OwnerController->GetBlackboard()->SetValueAsFloat(Keys::HeardSomethingOnPatrolWaitTime, 0);
		OwnerBlackboard->SetValueAsBool(Keys::IsRotating,false);
		//WOwnerController->GetBlackboard()->SetValueAsBool(Keys::HeardSomethingOnPatrol,false);

	}
	//Keys::HeardSomethingOnPatrol
	//if(OwnerController->GetBlackboard()->GetValueAsFloat(Keys::HeardSomethingOnPatrolWaitTime) == 0.f)
	//{
	//	bLookAtStimuliIsValid = false;
	//}
	//if(bLookAtSomething == true)
	//{
	//	LookAtStimuliSource(LookAtStumuli.StimulusLocation);
	//}

}

void UPestilenceHearingComponent::ConfigHearing() const
{
	const TObjectPtr<UAIPerceptionComponent> PerceptionComponent = OwnerController->PerceptionComponent;
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->RequestStimuliListenerUpdate();
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&UPestilenceHearingComponent::ProcessHearingStimuli);
}

void UPestilenceHearingComponent::ProcessHearingStimuli(AActor* Actor, FAIStimulus Stimulus)
{
	
	if(OwnerController->GetCurrentState() == EPestilenceStates::Dead) return;
	if(Stimulus.Type != HearingConfig->GetSenseID()) return;

	const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(!IsValid(PlayerController->GetPawn()) || !IsValid(OwnerController->GetBlackboard()) || !IsValid(OwnerController->GetPawn())) return;

	if( Actor == PlayerController->GetPawn() ||
		IsValid(Actor->FindComponentByClass<UInteractableComponent>())
		)
		if (Stimulus.WasSuccessfullySensed()) 
		{
			if((Stimulus.Tag.Compare(SoundTypeToString(SoundType::Step)) == 0) &&
				(OwnerController->GetPawn()->FindComponentByClass<UAIReactionComponent>()->GetType() == ECharacterType::Pawn) )
			{
				bLookAtStimuliIsValid = false;
				return;
			}
			//To force abort of the bt branch
			OwnerController->GetBlackboard()->SetValueAsBool(Keys::HeardSomethingOnPatrol,false);
			OwnerController->GetBlackboard()->SetValueAsBool(Keys::HeardSomethingOnPatrol,true);
			OwnerController->GetBlackboard()->SetValueAsFloat(
						Keys::HeardSomethingOnPatrolWaitTime,
						LookAtTimerReset
						);
			
			LastSoundType = Stimulus.Tag;
			
			bLookAtStimuliIsValid = true;
			LookAtStumuli = Stimulus;
			LookAtTimer = 0.f;
		}
		else if (EPestilenceStates(OwnerController->GetBlackboard()->GetValueAsEnum(Keys::State)) == EPestilenceStates::Chase) 
		{
			bLookAtStimuliIsValid = false;
		}
}

bool UPestilenceHearingComponent::LookAtStimuliSource(FVector StimuliLocation)
{
	if(OwnerController->GetCurrentState() == EPestilenceStates::Dead) return false;
	//TObjectPtr<AActor> OwnerPawn = OwnerController->GetPawn();
	TObjectPtr<AActor> OwnerPawn = OwnerController;
	TObjectPtr<AActor> OwnerPawn2 = OwnerController->GetPawn();
	if(!IsValid(OwnerPawn)) return false;
	TObjectPtr<UBlackboardComponent> OwnerBlackboard = OwnerController->GetBlackboard();
	if(!IsValid(OwnerBlackboard)) return false;
	
	FRotator PawnRot = UKismetMathLibrary::FindLookAtRotation(
		OwnerPawn->GetActorLocation(),
		StimuliLocation
		);
	PawnRot.Pitch = PawnRot.Roll = 0.f;
	const FRotator NewRot = FMath::RInterpTo(
		OwnerPawn->GetActorRotation(),
		PawnRot,
		GetWorld()->GetDeltaSeconds(),
		RotationSpeed
		);
	if(!PawnRot.Equals(NewRot,0.1f)/*PawnRot != NewRot*/)
	{
		OwnerPawn2->SetActorRotation(NewRot);
		OwnerPawn->SetActorRotation(NewRot);
		OwnerBlackboard->SetValueAsBool(Keys::IsRotating,true);

		return true;
	}
	bLookAtStimuliIsValid = false;
	OwnerBlackboard->SetValueAsBool(Keys::IsRotating,false);
	
	bLookAtStimuliIsValid = false;
	//OwnerController->GetBlackboard()->SetValueAsBool(Keys::HeardSomethingOnPatrol,false);
	OwnerBlackboard->SetValueAsBool(Keys::IsRotating,false);
	return false;
	
}

bool UPestilenceHearingComponent::CheckForHearingState(EPestilenceStates& CurrentState) const
{
	if(!IsValid(OwnerController->GetBlackboard())) return false;
	const EPestilenceStates AICurrentState = EPestilenceStates(OwnerController->GetBlackboard()->GetValueAsEnum(Keys::State));
	CurrentState = AICurrentState;
	if (!IsValid(OwnerController->GetPawn())) return false;
	
	if( AICurrentState != EPestilenceStates::Stun &&
		AICurrentState != EPestilenceStates::Distracted &&
		AICurrentState != EPestilenceStates::Alert &&
		AICurrentState != EPestilenceStates::Chase)
		return true;
	return false;
}

FName UPestilenceHearingComponent::SoundTypeToString(SoundType Sound)
{
	switch(Sound)
	{
		case SoundType::Step: 			return TEXT("Step");
		case SoundType::BottleBraking: 	return TEXT("BottleBreaking");
		case SoundType::Infection:		return TEXT("Infection");
		default:
			GEngine->AddOnScreenDebugMessage(40,20,FColor::Red,TEXT("The sound type does not exist"));
			return TEXT("Error");
	}
}