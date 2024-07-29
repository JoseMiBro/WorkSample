// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceLookAtComponent.h"

#include "PestilenceSightComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/PestilenceAIDoormanController.h"
#include "Pestilence/AI/PestilenceTargetPoint.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Animation/PestilenceCharacter/PestilenceCharacterAnimInstance.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/AI/PestilenceAIComponent.h"
#include "Pestilence/Props/ArchetypeZone.h"

// Sets default values for this component's properties
UPestilenceLookAtComponent::UPestilenceLookAtComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPestilenceLookAtComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	OwnerController = Cast<APestilenceAIController>(GetOwner());
	ControllerSightComponent = OwnerController->FindComponentByClass<UPestilenceSightComponent>();
	if(IsValid(OwnerController->GetPawn()))
	{
		CharacterReactionComponent = OwnerController->GetPawn()->FindComponentByClass<UAIReactionComponent>();
		AnimInstance = Cast<UPestilenceCharacterAnimInstance>(
			Cast<APestilenceCharacter>(
				OwnerController->GetPawn()
				)
				->GetMesh()->GetAnimInstance()
				);
	}

	LookStraight();

}


// Called every frame
void UPestilenceLookAtComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(!IsValid(OwnerController->GetBlackboard())) return;
	if(!IsValid(CharacterReactionComponent)) return;
	const EPestilenceStates ActualState = (EPestilenceStates)OwnerController->GetBlackboard()->GetValueAsEnum(Keys::State);
	const ECharacterType CharacterType = CharacterReactionComponent->GetType();
	TObjectPtr<AActor> PlayerCharacter;
	
	{
		if(CharacterType == ECharacterType::Foreman ||
			ActualState == EPestilenceStates::Doorman ||
			(	OwnerController->GetIsInForbiddenZone() ||
				OwnerController->GetIntegrityFactor() == 1.f ||
				OwnerController->GetBlackboard()->GetValueAsBool(Keys::Witness)
			)
		)
		{
			switch (ActualState)
			{
				case EPestilenceStates::Patrol:
					if(ControllerSightComponent->bOnSight || ControllerSightComponent->IsOnCloseConeOfVision)
					{
						TurnHead();
					}
					else
					{
						if(!OwnerController->GetBlackboard()->GetValueAsBool(Keys::IsRotating))
							LookStraight();
						//OwnerController->GetBlackboard()->SetValueAsBool(Keys::IsRotating,false);
					}
					break;
				case EPestilenceStates::Alert:
				case EPestilenceStates::Chase:
				
				case EPestilenceStates::Watchdog:
					LookStraight();
					ControllerSightComponent->RotateToLookAtPlayer();
					OwnerController->GetBlackboard()->SetValueAsBool(Keys::IsRotating,true);
					break;
				case EPestilenceStates::Doorman:
					PlayerCharacter = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
					if(IsValid(PlayerCharacter))
					{
						const TObjectPtr<APestilenceAIDoormanController> NewController = Cast<APestilenceAIDoormanController>(OwnerController);
						const float DistanceToPlayer = FVector::DistSquared(NewController->WarningTargetPoint->GetActorLocation(),PlayerCharacter->GetActorLocation());
						const TObjectPtr<AArchetypeZone> Zone = NewController->GetCharacter()->FindComponentByClass<UPestilenceAIComponent>()->AssignedZone;

						if(Zone->ActorIsRestricted(PlayerCharacter))
						{
							if(NewController->FindComponentByClass<UPestilenceSightComponent>()->bOnSight)
							{
								TurnHead();
								if((DistanceToPlayer <= (NewController->GetWarningDistance() * NewController->GetWarningDistance())))
								{
									ControllerSightComponent->RotateToLookAtPlayer();
									OwnerController->GetBlackboard()->SetValueAsBool(Keys::IsRotating,true);
								}
								else
								{
									OwnerController->GetBlackboard()->SetValueAsBool(Keys::IsRotating,false);
								}
							}
							else
							{
								LookStraight();
								OwnerController->GetBlackboard()->SetValueAsBool(Keys::IsRotating,false);
							}
						}
						else
						{
							LookStraight();
							OwnerController->GetBlackboard()->SetValueAsBool(Keys::IsRotating,false);
						}
					}
					break;
					
				default:
					LookStraight();
					OwnerController->GetBlackboard()->SetValueAsBool(Keys::IsRotating,false);
				break;

			}
		}
		else
		{
			LookStraight();
			OwnerController->GetBlackboard()->SetValueAsBool(Keys::IsRotating,false);
		}
	}
}

void UPestilenceLookAtComponent::TurnHead()
{
	const TObjectPtr<APawn> PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(),0)->GetPawn();
	const FVector LookAtPos = Cast<APestilenceCharacter>(PlayerPawn)->GetFirstPersonCameraComponent()->GetComponentLocation();
	if(!IsValid(PlayerPawn)) return;
	
	if(ControllerSightComponent->bOnSight)
	{
		LookAtPoint = LookAtPos;
		/* PlayerPawn->GetActorLocation()*/
		if(IsValid(AnimInstance))
		{
			AnimInstance->LookAtPoint = LookAtPoint;
			AnimInstance->MaxMovementAngle = MaxMovementAngle;
		}
	}
	else
	{
		LookStraight();
	}
}

void UPestilenceLookAtComponent::LookStraight()
{
	const TObjectPtr<APawn> OwnerPawn = OwnerController->GetPawn();
	if(!IsValid(OwnerPawn)) return;
	const TObjectPtr<UCameraComponent> Camera = Cast<APestilenceCharacter>(OwnerPawn)->GetFirstPersonCameraComponent();
	const FVector LookAtPos = Camera->GetComponentLocation();

	LookAtPoint = LookAtPos + (DefaultLookAtPointOffset.X * Camera->GetForwardVector() + DefaultLookAtPointOffset.Z);
	if(IsValid(AnimInstance))
	{
		AnimInstance->LookAtPoint = LookAtPoint;
		AnimInstance->MaxMovementAngle = MaxMovementAngle;
	}
}

