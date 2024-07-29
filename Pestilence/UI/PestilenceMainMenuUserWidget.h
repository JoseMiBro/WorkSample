	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Options/PestilenceOptionsMenu.h"
#include "PestilenceMainMenuUserWidget.generated.h"


class UButton;
class UCanvasPanel;
class APestilenceHUD; 
class UTextBlock;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmWindowOpen);


UENUM(BlueprintType)
	enum class EnumMain : uint8
{
	NewGame					UMETA(DisplayName = "NewGame"),
	Quit						UMETA(DisplayName = "Quit")
};

UCLASS()
class PESTILENCE_API UPestilenceMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
		UUserWidget* IntroTextWidgetInstance = nullptr;

	UPROPERTY(BlueprintAssignable)
	FOnConfirmWindowOpen OnConfirmWindowOpen;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> MenuMappingContext;

	UFUNCTION(BlueprintCallable)
	void SetFocusMain();
		
	UFUNCTION(BlueprintCallable)
	void RemoveMenuMappingContext();

private:
	UPROPERTY(EditAnywhere, Category = "IntroText")
		TSubclassOf<UUserWidget> IntroTextWidgetClass;

public:
	//Buttons
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UUserWidget* StartNewGameButton; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UPestilenceOptionsMenu* WBP_OptionsMenu;

protected:
	//Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ContinueGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* LoadGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* OptionsButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ExitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ConfirmationYesButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ConfirmationNoButton;

	//Canvas
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* MainMenuPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* ConfirmationPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* LoadGamePanel;

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
		UUserWidget* BackMainMenuButton;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* ConfimationText;

protected:
	UPROPERTY(BlueprintReadOnly)
		APestilenceHUD* PestilenceHUD;

		UPROPERTY(BlueprintReadOnly)
		EnumMain enumMain;

		UPROPERTY(EditAnywhere, Category = "MainMenu")
		FString ConfirmationExit = "Are you sure you want to EXIT?";

		UPROPERTY(EditAnywhere, Category = "MainMenu")
		FString ConfirmationNewGame = "Are you sure you want to DELETE the QUICK SAVE file?";

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
		TArray<UWidget*> GetAllWidgetsRecursively();

	UFUNCTION(BlueprintCallable)
    void Click(UUserWidget* Button);
  
	UFUNCTION(BlueprintCallable)
  void LoadGame();

protected:
  UFUNCTION()
    void ContinueGame();

	UFUNCTION()
		void LoadSaveSlotsGame();

	UFUNCTION()
		void StartNewGame();

	UFUNCTION(BlueprintCallable)
		void Options();

	UFUNCTION()
		void Exit();

	UFUNCTION()
		void ConfirmationYes();

	UFUNCTION()
		void ConfirmationNo();

		void NewGame();

		void SetConfirmationPanel(FString _ConfirmationText, EnumMain _EnumMain);
};
