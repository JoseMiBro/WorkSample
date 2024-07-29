// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceDFUserWidget.generated.h"

UCLASS()
class PESTILENCE_API UPestilenceDFUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnImportantModified();
};
