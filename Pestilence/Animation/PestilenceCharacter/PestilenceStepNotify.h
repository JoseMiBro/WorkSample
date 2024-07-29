// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PestilenceStepNotify.generated.h"

class USoundBase;
class USoundAttenuation;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceStepNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
  UPROPERTY(EditAnywhere, Category = AnimNotify)
  FName SocketName;
  UPROPERTY(EditAnywhere, Category = AnimNotify, Meta = (ClampMin = "0", ClampMax = "1"))
  float VolumeMultiplier;
  UPROPERTY(EditAnywhere, Category = AnimNotify, Meta = (ClampMin = "0", ClampMax = "1"))
  float PitchMultiplier;
  UPROPERTY(EditAnywhere, Category = AnimNotify)
  float StartTime;
};
