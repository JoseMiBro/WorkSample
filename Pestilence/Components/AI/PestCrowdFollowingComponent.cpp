// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Components/AI/PestCrowdFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavMesh/RecastNavMesh.h"
#include "Pestilence/AI/NavArea/NavArea_Interact.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

UPestCrowdFollowingComponent::UPestCrowdFollowingComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SimulationState = ECrowdSimulationState::Enabled;
		
	bAffectFallingVelocity = false;
	bRotateToVelocity = true;
	bSuspendCrowdSimulation = false;
	bEnableSimulationReplanOnResume = true;
	bRegisteredWithCrowdSimulation = false;
	bCanCheckMovingTooFar = true;
	bCanUpdatePathPartInTick = true;

	bEnableAnticipateTurns = false;
	bEnableObstacleAvoidance = true;
	bEnableSeparation = true;
	bEnableOptimizeVisibility = true;
	bEnableOptimizeTopology = true;
	bEnablePathOffset = false;
	bEnableSlowdownAtGoal = true;

	SeparationWeight = 100.0f;
	CollisionQueryRange = 35.f * 12.f;		// approx: radius * 12.0f
	PathOptimizationRange = 35.f * 30.f;	// approx: radius * 30.0f
	AvoidanceQuality = ECrowdAvoidanceQuality::High;
	AvoidanceRangeMultiplier = 1.0f;
}

void UPestCrowdFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);

	// if (CharacterMovementComp != nullptr)
	// {
	// 	const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];
	//
	// 	if (FNavAreaHelper::HasJumpFlag(SegmentStart))
	// 	{
	// 		CharacterMovementComp->SetMovementMode(MOVE_Flying);
	// 	}
	// 	else if (FNavAreaHelper::HasInteractFlag(SegmentStart))
	// 	{
	// 		// if(!IsValid(PestilenceCharacter))
	// 		// {
	// 		// 	PestilenceCharacter = Cast<APestilenceCharacter>(MovementComp->GetOwner());
	// 		// }
	// 		// if(IsValid(PestilenceCharacter) && bCanUse == true)
	// 		// {
	// 		// 	PestilenceCharacter->AIInteract();
	// 		// 	CharacterMovementComp->SetMovementMode(MOVE_None);
	// 		// 	bCanUse = false;
	// 		// }
	// 	}
	// 	else if (!FNavAreaHelper::HasJumpFlag(SegmentStart) && !FNavAreaHelper::HasInteractFlag(SegmentStart))
	// 	{
	// 		// regular move
	// 		CharacterMovementComp->SetMovementMode(MOVE_Walking);
	// 		bCanUse = true;
	// 	}
	// }
}

void UPestCrowdFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
	Super::SetMovementComponent(MoveComp);

	CharacterMovementComp = Cast<UCharacterMovementComponent>(MovementComp);
}

void UPestCrowdFollowingComponent::OnNavNodeChanged(NavNodeRef NewPolyRef, NavNodeRef PrevPolyRef, int32 CorridorSize)
{
	Super::OnNavNodeChanged(NewPolyRef, PrevPolyRef, CorridorSize);

	const ARecastNavMesh* NavMesh = Path.IsValid() ? Cast<ARecastNavMesh>(Path->GetNavigationDataUsed()) : nullptr;

	if (NavMesh)
	{
		FNavMeshNodeFlags Flags;
		NavMesh->GetPolyFlags(NewPolyRef, Flags);

		FVector LinkA;
		FVector LinkB;
		
		if (Flags.IsNavLink() && NavMesh->GetLinkEndPoints(NewPolyRef, LinkA, LinkB))
		{
			const UClass* AreaClass = NavMesh->GetAreaClass(Flags.Area);

			if (AreaClass == UNavArea_Interact::StaticClass())
			{
				PestilenceCharacter = Cast<APestilenceCharacter>(MovementComp->GetOwner());
			
				if(IsValid(PestilenceCharacter) && bCanUse == true)
				{
					PestilenceCharacter->AIInteract();
					bCanUse = false;
				}
			}
		}
		else
		{
			bCanUse = true;
		}
	}
}
