// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayBehavior.h"
#include "GameplayBehavior_Pestilence.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PESTILENCE_API UGameplayBehavior_Pestilence : public UGameplayBehavior
{
	GENERATED_BODY()
public:
	UGameplayBehavior_Pestilence(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
