#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Pestilence/InteractiveActors/InteractiveActor.h"
#include "PestilencePathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilencePathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(Transient)
		UCharacterMovementComponent* CharacterMovementComp;

public:
	// used to detect properties of a path's segment a character is about to follow
	virtual void SetMoveSegment(int32 SegmentStartIndex) override;

	// used to cache UCharacterMovementComponent we're using in SetMoveSegment implementation
	virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AInteractiveActor> Door;
};
