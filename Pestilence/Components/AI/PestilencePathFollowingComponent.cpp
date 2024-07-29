#include "Pestilence/Components/AI/PestilencePathFollowingComponent.h"

#include "PestilenceAIComponent.h"
#include "Pestilence/AI/Enums/ENavAreaFlag.h"

void UPestilencePathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);

	if (CharacterMovementComp != NULL)
	{
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];

		if (FNavAreaHelper::HasJumpFlag(SegmentStart))
		{
			CharacterMovementComp->SetMovementMode(MOVE_Flying);
		}
		else if (FNavAreaHelper::HasInteractFlag(SegmentStart))
		{
			CharacterMovementComp->SetMovementMode(MOVE_None);
			// open door

			if(bWalkingNavLinkStart)
			{
				//puerta
			}
			
			CharacterMovementComp->SetMovementMode(MOVE_Walking);
		}
		else if (!FNavAreaHelper::HasJumpFlag(SegmentStart) && !FNavAreaHelper::HasInteractFlag(SegmentStart))
		{
			// regular move
			CharacterMovementComp->SetMovementMode(MOVE_Walking);
		}
	}
}

void UPestilencePathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
	Super::SetMovementComponent(MoveComp);

	CharacterMovementComp = Cast<UCharacterMovementComponent>(MovementComp);
}