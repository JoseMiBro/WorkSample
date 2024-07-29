// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "PestCrowdFollowingComponent.generated.h"

class APestilenceCharacter;
class UCharacterMovementComponent;

UCLASS()
class PESTILENCE_API UPestCrowdFollowingComponent : public UCrowdFollowingComponent
{
	GENERATED_BODY()
	UPestCrowdFollowingComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void SetMoveSegment(int32 SegmentStartIndex) override;
	virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;

	virtual void OnNavNodeChanged(NavNodeRef NewPolyRef, NavNodeRef PrevPolyRef, int32 CorridorSize) override;

protected:
	UPROPERTY(Transient)
	UCharacterMovementComponent* CharacterMovementComp;

	UPROPERTY(EditAnywhere)
	APestilenceCharacter* PestilenceCharacter;

	UPROPERTY(VisibleAnywhere)
	bool bCanUse = true;
};
