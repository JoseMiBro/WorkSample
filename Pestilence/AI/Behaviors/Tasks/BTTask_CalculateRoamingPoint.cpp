#include "Pestilence/AI/Behaviors/Tasks/BTTask_CalculateRoamingPoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

UBTTask_CalculateRoamingPoint::UBTTask_CalculateRoamingPoint()
{
	NodeName = "CalculateRoamingPoint";
}

EBTNodeResult::Type UBTTask_CalculateRoamingPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const FName BlackboardKeyName = BlackboardKey.SelectedKeyName;
	const FVector Position = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKeyName);
	const FName BlackboardResultName = BlackboardResultKey.SelectedKeyName;

	TObjectPtr<APestilenceCharacter> PestilenceCharacter = Cast<APestilenceCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (!IsValid(PestilenceCharacter))
		return EBTNodeResult::Failed;
	
	FVector LastPlayerMovementDirection = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Keys::PlayerMovementDirection);
	if(LastPlayerMovementDirection.IsNearlyZero()) return EBTNodeResult::Failed;
	FVector StartLocation = PestilenceCharacter->GetActorLocation() + PestilenceCharacter->GetActorForwardVector() * OffsetDistance;
	FVector ForwardVector  = PestilenceCharacter->GetActorForwardVector();
	FVector LeftVector  = PestilenceCharacter->GetActorRightVector() * -1.f;
	FVector RightVector  = PestilenceCharacter->GetActorRightVector();
	
	FHitResult HitResult;

	bool bHitPlayerDirection = UKismetSystemLibrary::LineTraceSingle(
		this,
		StartLocation,
		StartLocation + LastPlayerMovementDirection * RaycastLength,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	
	bool bHitForward = UKismetSystemLibrary::LineTraceSingle(
		this,
		StartLocation,
		StartLocation + ForwardVector * RaycastLength,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	bool bHitLeft = UKismetSystemLibrary::LineTraceSingle(
		this,
		StartLocation,
		StartLocation + LeftVector * RaycastLength,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	bool bHitRight = UKismetSystemLibrary::LineTraceSingle(
		this,
		StartLocation,
		StartLocation + RightVector * RaycastLength,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	FNavLocation WanderingPoint;
	FNavLocation LKPNavLocation;

	FVector NewCheckLocation;
	FVector QueryingExtent = FVector(50.0f, 50.0f, 250.0f);

	if (bHitPlayerDirection == false || bHitForward == false)
	{
		if(UWorld* World = GetWorld())
		{
			const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(World);

			ANavigationData* NavigationData = NavigationSystem->MainNavData;
			
			NewCheckLocation = Position + (LastPlayerMovementDirection * DistancePoint);

			if(NavigationSystem->GetRandomPointInNavigableRadius(NewCheckLocation, Radius, WanderingPoint))
			{
#if WITH_EDITOR
				if (bDrawDebug)
				{
					DrawDebugSphere(World, NewCheckLocation, Radius, 12, FColor::Green, false, 3.f);
					DrawDebugSphere(World, NewCheckLocation, 30.f, 12.f, FColor::White, false, 15.f);
				}
#endif
				if (NavigationSystem->ProjectPointToNavigation(NewCheckLocation, LKPNavLocation, QueryingExtent, NavigationData))
				{
					// Comprueba si la proyección está dentro de la NavMesh
					if (LKPNavLocation.NodeRef != INVALID_NAVNODEREF)
					{
						//Point in NavMesh
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(Keys::LastKnownPoint, NewCheckLocation);
						//DrawDebugSphere(World, NewCheckLocation, 30.f, 12.f, FColor::Silver, false, 15.f);
					}
				}
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardResultName, WanderingPoint.Location);
			}
			return EBTNodeResult::Succeeded;
		}
	}
	
	if (bHitForward)
	{
		if (bHitLeft && bHitRight || !bHitLeft && !bHitRight)
		{
			Direction = FVector::ZeroVector;
		}
		else if (bHitRight && !bHitLeft)
		{
			Direction = LeftVector;
		}
		else if (!bHitRight && bHitLeft)
		{
			Direction = RightVector;
		}
	}

	if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
	{
		UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *HitResult.GetActor()->GetName());
	}

	if(UWorld* World = GetWorld())
	{
		const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(World);

		ANavigationData* NavigationData = NavigationSystem->MainNavData;

		NewCheckLocation = Position + (Direction * DistancePoint);
		
		if(NavigationSystem->GetRandomPointInNavigableRadius(NewCheckLocation, Radius, WanderingPoint))
		{
#if WITH_EDITOR
			if (bDrawDebug)
			{
				DrawDebugSphere(World, NewCheckLocation, Radius, 12, FColor::Orange, false, 3.f);
				DrawDebugSphere(World, Position + (LastPlayerMovementDirection * DistancePoint), 30.f, 12.f, FColor::White, false, 15.f);
			}
#endif
			if (NavigationSystem->ProjectPointToNavigation(NewCheckLocation, LKPNavLocation, QueryingExtent, NavigationData))
			{
				// Comprueba si la proyección está dentro de la NavMesh
				if (LKPNavLocation.NodeRef != INVALID_NAVNODEREF)
				{
					//Point in NavMesh
			        //OwnerComp.GetBlackboardComponent()->SetValueAsVector(Keys::LastKnownPoint,Position + (LastPlayerMovementDirection * DistancePoint));
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(Keys::LastKnownPoint,NewCheckLocation);
					//DrawDebugSphere(World, NewCheckLocation, 30.f, 12.f, FColor::Silver, false, 15.f);
				}
			}
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardResultName, WanderingPoint.Location);
		}
	}
	return EBTNodeResult::Succeeded;
}

FString UBTTask_CalculateRoamingPoint::GetStaticDescription() const
{
	return FString("Calculate and set new RoamingPoint");
}
