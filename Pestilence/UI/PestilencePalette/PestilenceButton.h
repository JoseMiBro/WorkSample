// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "PestilenceButton.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceButton : public UButton
{
	GENERATED_BODY()
	
public:
	UPestilenceButton();

protected:
	virtual void PostLoad() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomButton")
		float MaxOpacity = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomButton")
		float MinOpacity = 0.5f;

	UFUNCTION(BlueprintCallable, Category = "CustomButton")
		void OnHoveredPestilence();

	UFUNCTION(BlueprintCallable, Category = "CustomButton")
		void OnUnhoveredPestilence();
};
