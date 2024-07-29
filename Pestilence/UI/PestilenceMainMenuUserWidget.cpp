
// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceMainMenuUserWidget.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#include "Pestilence/AI/PestilenceAIController.h"

#include "Pestilence/UI/PestilenceHUD.h"
#include "Pestilence/UI/Options/PestilenceOptionsMenu.h"
#include "Pestilence/Core/PestilenceGameInstance.h"
#include "Pestilence/Core/PestilenceSaveGameSubsystem.h"
#include "EnhancedInputSubsystems.h"

#include "Blueprint/WidgetTree.h"


void UPestilenceMainMenuUserWidget::SetFocusMain()
{
	TArray<UWidget*> Children;

	StartNewGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->SetFocus();
		}
	}
	Children.Empty();
}

void UPestilenceMainMenuUserWidget::RemoveMenuMappingContext()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(MenuMappingContext);
	}
}

void UPestilenceMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ConfirmationNo();
}

void UPestilenceMainMenuUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> Children;

	ContinueGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceMainMenuUserWidget::ContinueGame);
		}
	}
	Children.Empty();

	StartNewGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceMainMenuUserWidget::StartNewGame);
		}
	}
	Children.Empty();

	LoadGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceMainMenuUserWidget::LoadSaveSlotsGame);
		}
	}
	Children.Empty();

	OptionsButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceMainMenuUserWidget::Options);
		}
	}
	Children.Empty();

	ConfirmationYesButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceMainMenuUserWidget::ConfirmationYes);
		}
	}
	Children.Empty();

	ConfirmationNoButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceMainMenuUserWidget::ConfirmationNo);
		}
	}
	Children.Empty();

	BackMainMenuButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceMainMenuUserWidget::ConfirmationNo);
		}
	}
	Children.Empty();
}

bool UPestilenceMainMenuUserWidget::Initialize()
{
	bool parentSuccess = Super::Initialize();
	if (!parentSuccess)
	{
		return parentSuccess;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//StartNewGameButton->SetUserFocus(PlayerController);

	if (GetWorld())
	{
		if (PlayerController && PlayerController->GetHUD())
		{
			PestilenceHUD = Cast<APestilenceHUD>(PlayerController->GetHUD());
		}
	}

	return parentSuccess;
}

void UPestilenceMainMenuUserWidget::ContinueGame()
{
	UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
	UPestilenceSaveGameSubsystem* SGSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
	SGSubsystem->SetCheckPointSlotName();
	if (UGameplayStatics::DoesSaveGameExist(SGSubsystem->GetCurrentSlotName(), 0))
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			FInputModeGameOnly InputMode;
			InputMode.SetConsumeCaptureMouseDown(true);

			PlayerController->bShowMouseCursor = false;

			PlayerController->SetInputMode(InputMode);
		}
		GameInstance->SetLoadGame(true);
	}
	else
	{
		SGSubsystem->QuickLoad(); //Gives nosave debug message
		SGSubsystem->ResetSlotName();
	}
}

void UPestilenceMainMenuUserWidget::LoadSaveSlotsGame()
{
	LoadGamePanel->SetVisibility(ESlateVisibility::Visible);
	MainMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);

	Slot_1LoadButton->SetFocus();
}

void UPestilenceMainMenuUserWidget::StartNewGame()
{
	UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
	UPestilenceSaveGameSubsystem* SGSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
	SGSubsystem->ResetSlotName();
	if (UGameplayStatics::DoesSaveGameExist(SGSubsystem->GetCurrentSlotName(), 0))
	{
		SetConfirmationPanel(ConfirmationNewGame, EnumMain::NewGame);
		//LanzarPanelCofirmacion
		//Perder datos guardo rapido
		//UGameplayStatics::DeleteGameInSlot(SGSubsystem->GetCurrentSlotName(), 0);
	}

	else 
	{
		NewGame();
	}

	//if (IntroTextWidgetClass)
	//{
	//	IntroTextWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), IntroTextWidgetClass);
	//	if (IntroTextWidgetInstance)
	//	{
	//		PestilenceHUD->HideWidget(this);
	//		IntroTextWidgetInstance->AddToViewport(1);
	//
	//		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//	}
	//}
}

void UPestilenceMainMenuUserWidget::Options()
{
	WBP_OptionsMenu->OnOpenOptions.Broadcast();
	if (PestilenceHUD)
	{
		//PestilenceHUD->ShowWidget(PestilenceHUD->OptionsWidgetInstance);
    //PestilenceHUD->OptionsWidgetInstance->OptionGoMenu = EnumOptionGoMenu::MainMenu;
		//PestilenceHUD->OptionsWidgetInstance->SetOptionsFocusInit();
    //MainMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPestilenceMainMenuUserWidget::Exit()
{
	SetConfirmationPanel(ConfirmationExit, EnumMain::Quit);
}

void UPestilenceMainMenuUserWidget::ConfirmationYes()
{
	switch (enumMain)
	{
	case EnumMain::NewGame: 
	{
		UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
		UPestilenceSaveGameSubsystem* SGSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
		SGSubsystem->SetCheckPointSlotName();
		UGameplayStatics::DeleteGameInSlot(SGSubsystem->GetCurrentSlotName(), 0);
		NewGame();
	}
		break;
	case EnumMain::Quit: 
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		playerController->ConsoleCommand("quit");
	}
		break;
	default:
		break;
	}
}

void UPestilenceMainMenuUserWidget::ConfirmationNo()
{
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	LoadGamePanel->SetVisibility(ESlateVisibility::Hidden);
	MainMenuPanel->SetVisibility(ESlateVisibility::Visible);
	StartNewGameButton->SetFocus();
}

void UPestilenceMainMenuUserWidget::NewGame()
{
	RemoveMenuMappingContext();
	if (PestilenceHUD) 
	{
		FName LevelName(*PestilenceHUD->StartNewGameSceneName);
		UGameplayStatics::OpenLevel(this, LevelName);

		if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == nullptr)
		{
			return;
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			FInputModeGameOnly InputMode;
			InputMode.SetConsumeCaptureMouseDown(true);

			PlayerController->bShowMouseCursor = false;

			PlayerController->SetInputMode(InputMode);
		}
	}
}

void UPestilenceMainMenuUserWidget::SetConfirmationPanel(FString _ConfirmationText, EnumMain _EnumMain)
{
	ConfirmationPanel->SetVisibility(ESlateVisibility::Visible);

	ConfirmationNoButton->SetFocus();
	ConfimationText->SetText(FText::FromString(_ConfirmationText));
	enumMain = _EnumMain;
	OnConfirmWindowOpen.Broadcast();
}

TArray<UWidget*> UPestilenceMainMenuUserWidget::GetAllWidgetsRecursively()
{
	TArray<UWidget*> Children;
	this->WidgetTree->GetAllWidgets(Children);
	return(Children);
}

void UPestilenceMainMenuUserWidget::Click(UUserWidget* Button)
{
	TArray<UWidget*> Children;
	Button->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.Broadcast();
			break;
		}
	}

}

void UPestilenceMainMenuUserWidget::LoadGame()
{
  UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
  UPestilenceSaveGameSubsystem* SGSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
  if (UGameplayStatics::DoesSaveGameExist(SGSubsystem->GetCurrentSlotName(), 0))
  {
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (PlayerController)
    {
      FInputModeGameOnly InputMode;
      InputMode.SetConsumeCaptureMouseDown(true);

      PlayerController->bShowMouseCursor = false;

      PlayerController->SetInputMode(InputMode);
    }
    GameInstance->SetLoadGame(true);
  }
  else
  {
    SGSubsystem->QuickLoad(); //Gives nosave debug message
    SGSubsystem->ResetSlotName();
  }
}
