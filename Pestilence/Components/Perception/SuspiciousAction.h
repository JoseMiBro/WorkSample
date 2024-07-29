#pragma once
#include "Pestilence/Enums/SuspiciousActionType.h"
#include "SuspiciousAction.generated.h"

USTRUCT(BlueprintType)
struct FSuspiciousAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESuspiciousActionType SuspiciousType = ESuspiciousActionType::Default;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Points = 0; //TODO: If this grows pack this in struct

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> SenderActor = nullptr;
};