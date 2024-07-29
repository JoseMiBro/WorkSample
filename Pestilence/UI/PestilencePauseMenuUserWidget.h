// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilencePauseMenuUserWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class APestilenceHubManager;
class APestilenceHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmWindow);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContinueGame);

UENUM(BlueprintType)
enum class EnumPause : uint8
{
	Restart					UMETA(DisplayName = "Restart"),
	Quit						UMETA(DisplayName = "Quit"),
	BackMainMenu		UMETA(DisplayName = "BackMainMenu"),
	FirstSlot				UMETA(DisplayName = "FirstSlot"),
	SecondSlot			UMETA(DisplayName = "SecondSlot"),
	ThirdSlot				UMETA(DisplayName = "ThirdSlot")
};

UCLASS()
class PESTILENCE_API UPestilencePauseMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		bool binViewport = false;
	//Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ContinueGameButton;

	UPROPERTY(BlueprintAssignable)
		FOnConfirmWindow OnConfirmWindow;

	UPROPERTY(BlueprintAssignable)
		FOnContinueGame OnContinueGame;

protected:
	//Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* LastCheckGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* LoadSaveGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* SaveGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ReloadGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* OptionsButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* MainMenuGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ExitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ConfirmationYesButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ConfirmationNoButton;

	//Canvas
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* PauseMenuPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* ConfirmationPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* LoadGamePanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* SaveGamePanel;

	//Save Canvas
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_1Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_2Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_3Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* BackPauseMenuButton;

	//Save Canvas
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_1LoadButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_2LoadButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_3LoadButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_4LoadButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* Slot_5LoadButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* BackPauseMenuLoadButton;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ConfimationText;

	//Other
protected:
	UPROPERTY(BlueprintReadOnly)
		APestilenceHubManager* m_PestilenceHubManager;

	UPROPERTY(BlueprintReadOnly)
		APestilenceHUD* PestilenceHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PauseMenu")
		EnumPause enumPause;

private:
	UPROPERTY(EditAnywhere, Category = "PauseMenu")
		FString ConfirmationRestart = "Are you sure you want to RESTARTLEVEL?";

	UPROPERTY(EditAnywhere, Category = "PauseMenu")
		FString ConfirmationExit = "Are you sure you want to EXIT?";

	UPROPERTY(EditAnywhere, Category = "PauseMenu")
		FString ConfirmationBackMainMenu = "Are you sure you want to BACKTOMAINMENU?";

	UPROPERTY(EditAnywhere, Category = "PauseMenu")
		FString ConfirmationSlot1 = "Are you sure you save SLOT 1?";

	UPROPERTY(EditAnywhere, Category = "PauseMenu")
		FString ConfirmationSlot2 = "Are you sure you save SLOT 2?";

	UPROPERTY(EditAnywhere, Category = "PauseMenu")
		FString ConfirmationSlot3 = "Are you sure you save SLOT 3?";

  UPROPERTY(EditAnywhere, Category = "PauseMenu")
  TObjectPtr<USoundBase> PressButtonSound;
  UPROPERTY(EditAnywhere, Category = "PauseMenu")
  TObjectPtr<USoundBase> SelectButtonSound;
  UPROPERTY(EditAnywhere, Category = "PauseMenu")
  TObjectPtr<USoundBase> BackSound;
public:
	virtual bool Initialize() override;
	UFUNCTION(BlueprintCallable)
		void ContinueGame();
	UFUNCTION(BlueprintCallable)
		void Click(UUserWidget* Button);
	UFUNCTION(BlueprintCallable)
		TArray<UWidget*> GetAllWidgetsRecursively();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
		void LastCheckGame();

	UFUNCTION()
		void SaveGame();

	UFUNCTION()
		void LoadSaveGame();

	UFUNCTION()
		void BackMainMenu();

	UFUNCTION()
		void ReloadGame();

	UFUNCTION()
		void Options();

	UFUNCTION()
		void Exit();

	UFUNCTION()
		void SetConfirmationPanel(FString _ConfirmationText, EnumPause _EnumPause);

	UFUNCTION()
		void ConfirmationYes();

	UFUNCTION()
		void ConfirmationNo();

	UFUNCTION()
		void SlotSave1();

	UFUNCTION()
		void SlotSave2();

	UFUNCTION()
		void SlotSave3();
};
