#include "Pestilence/AI/Behaviors/Services/UpdateLKP.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

UUpdateLKP::UUpdateLKP()
{
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
	NodeName = TEXT("Update LKP");
}

void UUpdateLKP::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

// 	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());
// 	
// 	const TObjectPtr<APlayerController> PestilenceCharacter = Cast<APlayerController>(PestilenceAIController->GetBlackboard()->GetValueAsObject(Keys::Player));
//
// 	if (PestilenceAIController == nullptr || PestilenceCharacter == nullptr)
// 		return;
//
// 	const EPestilenceStates AIState = static_cast<EPestilenceStates>(PestilenceAIController->GetBlackboard()->GetValueAsEnum(Keys::State));
// 	const bool Witness = PestilenceAIController->GetBlackboard()->GetValueAsBool(Keys::Witness);
// 	const FVector LastKnownPoint = PestilenceCharacter->GetPawn()->GetActorLocation();
//
// 	if(!PestilenceAIController->GetIsInForbiddenZone() || (
// 		AIState == EPestilenceStates::Chase)
// 		 || Witness )
// 	{
// 		PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, LastKnownPoint);
// 		PestilenceAIController->GetBlackboard()->SetValueAsBool(Keys::LKPReached, false);
// 	}
}

void UUpdateLKP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());

	const TObjectPtr<APlayerController> PestilenceCharacter = Cast<APlayerController>(PestilenceAIController->GetBlackboard()->GetValueAsObject(Keys::Player));
	
	if (PestilenceAIController == nullptr || PestilenceCharacter == nullptr)
		return;

	//Para Jose: Añado el if porque diseño no quiere que el LKP se actualize en zonas restringidas 
	const EPestilenceStates AIState = static_cast<EPestilenceStates>(PestilenceAIController->GetBlackboard()->GetValueAsEnum(Keys::State));
	const bool Witness = PestilenceAIController->GetBlackboard()->GetValueAsBool(Keys::Witness);
	const bool SeeingPlayer = PestilenceAIController->GetBlackboard()->GetValueAsBool(Keys::SeeingPlayer);

	if (SeeingPlayer)
	{
		PestilenceAIController->CalculateIntegrityFactor();
		if(/*!PestilenceAIController->GetIsInForbiddenZone()
			||*/ Witness
			|| ((AIState == EPestilenceStates::Alert || AIState == EPestilenceStates::Chase) &&
				PestilenceAIController->GetIntegrityFactor() == 1.f))
		{
			const FVector LastKnownPoint = PestilenceCharacter->GetPawn()->GetActorLocation();
			const FVector PlayerOrientation = PestilenceCharacter->GetPawn()->GetActorForwardVector();

			const FVector CurrentVelocity = PestilenceCharacter->GetPawn()->GetVelocity();

			FVector MovementDirection = FVector::Zero();
			
			if (!CurrentVelocity.IsNearlyZero())
				MovementDirection = CurrentVelocity.GetSafeNormal();
			
			constexpr float ArrowSize = 100.f;

			FNavLocation LKPNavLocation;

			const FVector NewCheckLocation = LastKnownPoint;
			const FVector QueryingExtent = FVector(50.0f, 50.0f, 250.0f);

			UWorld* World = GetWorld();
			const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(World);

			const ANavigationData* NavigationData = NavigationSystem->MainNavData;
			
			if (NavigationSystem->ProjectPointToNavigation(NewCheckLocation, LKPNavLocation, QueryingExtent, NavigationData))
			{
				if (LKPNavLocation.NodeRef != INVALID_NAVNODEREF)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(Keys::LastKnownPoint, NewCheckLocation);
#if WITH_EDITOR
					if(bDrawDebug)
						DrawDebugSphere(GetWorld(), NewCheckLocation , 5.f, 5.f, FColor::Magenta, false, LifeTime);
#endif
				}
			}
			
			PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, LastKnownPoint);

			PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::PlayerOrientation, PlayerOrientation);
			
			PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::PlayerMovementDirection, MovementDirection);

#if WITH_EDITOR
			if(bDrawDebug)
			{
				DrawDebugDirectionalArrow(GetWorld(), LastKnownPoint, LastKnownPoint + PlayerOrientation * ArrowSize, 20.f, FColor::Green, false, LifeTime);
				DrawDebugDirectionalArrow(GetWorld(), LastKnownPoint, LastKnownPoint + MovementDirection * ArrowSize, 20.f, FColor::Red, false, LifeTime);
			}
#endif
		}
	}
}

FString UUpdateLKP::GetStaticServiceDescription() const
{
	return Super::GetStaticServiceDescription();
}