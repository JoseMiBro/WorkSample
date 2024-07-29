// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceIntegrityUserWidget.generated.h"

class UProgressBar;
class APestilenceCharacter;
UCLASS()
class PESTILENCE_API UPestilenceIntegrityUserWidget : public UUserWidget
{
	GENERATED_BODY()

//public:
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		UProgressBar* IntegrityBar;

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnIntegrityModified(float _PreviousIntegrity, float _CurrentIntegrity);

	UFUNCTION() 
	void OnEndPossess();

	UFUNCTION(BlueprintCallable)
	void SetPostProcess(float amount);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialParameterCollection> IntegrityParameterCollection;

	UPROPERTY(EditAnywhere)
	FName OffsetName;

private:
	void BindPlayerIntegrity();
	TObjectPtr<APestilenceCharacter> Player;
};
