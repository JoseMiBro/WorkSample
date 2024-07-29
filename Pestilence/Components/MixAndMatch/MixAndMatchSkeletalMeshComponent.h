// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MixAndMatchSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UMixAndMatchSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
protected:

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif 
};
