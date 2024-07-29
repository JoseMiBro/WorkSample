// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceFeedbackUserWidget.generated.h"

class UImage;
class UTexture2D;
class APestilenceCharacter;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceFeedbackUserWidget : public UUserWidget
{
	GENERATED_BODY()

//private:
	//UPROPERTY(meta = (BindWidget))
	//	UImage* FeedbackMovility;

	//UPROPERTY(meta = (BindWidget))
	//	UImage* FeedbackPlayerStealth;
	//
	//UPROPERTY(EditAnywhere, Category = "Movility")
	//	TArray<UTexture2D*> FeedbackMovilityTextures;
	//
	//UPROPERTY(EditAnywhere, Category = "PlayerStealth")
	//	TArray<UTexture2D*> FeedbackPlayerStealthTextures;

//protected:
//	virtual void NativeConstruct() override;

public:
  virtual bool Initialize() override;

  UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerInShadow(bool _bOnShadow);

  UFUNCTION()
  void OnEndPossess();

	//UFUNCTION()
	//	void OnPlayerRun(float speed);
	//
	//UFUNCTION()
  //	void OnPlayerCrouch(bool isCrouch);
private:
  void BindPlayer();
  TObjectPtr<APestilenceCharacter> Player;
};
