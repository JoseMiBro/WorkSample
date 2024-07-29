// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceGameOverUserWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class APestilenceHubManager;
class APestilenceGameMode;
class APestilenceHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmWindowOpenD);

UENUM(BlueprintType)
enum class EnumGameOver : uint8
{
	Restart					UMETA(DisplayName = "Restart"),
	Quit						UMETA(DisplayName = "Quit"),
	BackMainMenu		UMETA(DisplayName = "BackMainMenu")
};

UCLASS()
class PESTILENCE_API UPestilenceGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* LastCheckGameButton;

	UPROPERTY(BlueprintAssignable)
	FOnConfirmWindowOpenD OnConfirmWindowOpen;
protected:
	//Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ReloadGameButton; 
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* MainMenuGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ExitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ConfirmationYesButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ConfirmationNoButton;

	//Canvas
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* GameOverPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* ConfirmationPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* LoadGamePanel;

	//
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ConfimationText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* LoadSaveGameButton;

	//Save Canvas
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_1LoadButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_2LoadButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_3LoadButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_4LoadButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_5LoadButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* BackGameOverMenuButton;

protected:
	//Other
	UPROPERTY(BlueprintReadOnly)
		APestilenceHubManager* m_PestilenceHubManager;

	UPROPERTY(BlueprintReadOnly)
		APestilenceHUD* PestilenceHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameOverMenu")
		EnumGameOver enumGameOver;

private:
	UPROPERTY(EditAnywhere, Category = "GameOverMenu")
		FString ConfirmationRestart = "Are you sure you want to RESTARTLEVEL?";

	UPROPERTY(EditAnywhere, Category = "GameOverMenu")
		FString ConfirmationExit = "Are you sure you want to EXIT?";

	UPROPERTY(EditAnywhere, Category = "GameOverMenu")
		FString ConfirmationBackMainMenu = "Are you sure you want to BACKTOMAINMENU?";

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	TArray<UWidget*> GetAllWidgetsRecursively();

	UFUNCTION(BlueprintCallable)
	void Click(UUserWidget* Button);

private:
	UFUNCTION()
		void LastCheckGame();

	UFUNCTION()
		void BackMainMenu();

	UFUNCTION()
		void LoadSaveGameMenu();

	UFUNCTION()
		void ReloadGame();

	UFUNCTION()
		void Exit();

	UFUNCTION()
		void SetConfirmationPanel(FString _ConfirmationText, EnumGameOver _EnumGameOver);

	UFUNCTION()
		void ConfirmationYes();

	UFUNCTION()
		void ConfirmationNo();
};
