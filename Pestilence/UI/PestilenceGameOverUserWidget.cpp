// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceGameOverUserWidget.h"

#include "Kismet/GameplayStatics.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

#include "Pestilence/UI/PestilenceHUD.h"
//#include "Pestilence/UI/Options/PestilenceOptionsMenu.h"
//#include "Pestilence/UI/PestilenceHubManager.h"
#include <Pestilence/Core/PestilenceSaveGameSubsystem.h>
#include <Pestilence/Core/PestilenceGameInstance.h>

#include "Blueprint/WidgetTree.h"

void UPestilenceGameOverUserWidget::NativeConstruct()
{
	Super::NativeConstruct();ConfirmationNo();
}

void UPestilenceGameOverUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> Children;

	LastCheckGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceGameOverUserWidget::LastCheckGame);
		}
	}
	Children.Empty();

	MainMenuGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceGameOverUserWidget::BackMainMenu);
		}
	}
	Children.Empty();

	ReloadGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceGameOverUserWidget::ReloadGame);
		}
	}
	Children.Empty();

	LoadSaveGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceGameOverUserWidget::LoadSaveGameMenu);
		}
	}
	Children.Empty();

	ExitButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceGameOverUserWidget::Exit);
		}
	}
	Children.Empty();

	ConfirmationYesButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceGameOverUserWidget::ConfirmationYes);
		}
	}
	Children.Empty();

	ConfirmationNoButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceGameOverUserWidget::ConfirmationNo);
		}
	}
	Children.Empty();

	BackGameOverMenuButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceGameOverUserWidget::ConfirmationNo);
		}
	}
	Children.Empty();
}

bool UPestilenceGameOverUserWidget::Initialize()
{
	bool parentSuccess = Super::Initialize();
	if (!parentSuccess) 
	{
		return parentSuccess;
	}

	if (GetWorld())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController && PlayerController->GetHUD())
		{
			PestilenceHUD = Cast<APestilenceHUD>(PlayerController->GetHUD());
		}
	}

	return parentSuccess;
}

void UPestilenceGameOverUserWidget::LastCheckGame()
{
	UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
	UPestilenceSaveGameSubsystem* PestilenceSGS = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		FInputModeGameOnly InputMode;

		PlayerController->SetInputMode(InputMode);

		PlayerController->bShowMouseCursor = false;
		PlayerController->SetPause(false);
		RemoveFromParent();
	}

	GameInstance->LoadingScreenWidget->AddToViewport(999);
	GameInstance->SetLoadGame(true);
	PestilenceSGS->SetCheckPointSlotName();
}

void UPestilenceGameOverUserWidget::BackMainMenu()
{
  SetConfirmationPanel(ConfirmationRestart, EnumGameOver::BackMainMenu);
}

void UPestilenceGameOverUserWidget::LoadSaveGameMenu()
{
	LoadGamePanel->SetVisibility(ESlateVisibility::Visible);
	GameOverPanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);

	Slot_1LoadButton->SetFocus();
}

void UPestilenceGameOverUserWidget::ReloadGame()
{
  SetConfirmationPanel(ConfirmationRestart, EnumGameOver::Restart);
}

void UPestilenceGameOverUserWidget::Exit()
{
  SetConfirmationPanel(ConfirmationRestart, EnumGameOver::Quit);
}

void UPestilenceGameOverUserWidget::SetConfirmationPanel(FString _ConfirmationText, EnumGameOver _EnumGameOver)
{
  LoadGamePanel->SetVisibility(ESlateVisibility::Hidden);
  GameOverPanel->SetVisibility(ESlateVisibility::Hidden);
  ConfirmationPanel->SetVisibility(ESlateVisibility::Visible);

	ConfirmationNoButton->SetFocus();
  ConfimationText->SetText(FText::FromString(_ConfirmationText));
  enumGameOver = _EnumGameOver;
  OnConfirmWindowOpen.Broadcast();
}

void UPestilenceGameOverUserWidget::ConfirmationYes()
{
	switch (enumGameOver)
	{
	case EnumGameOver::Restart:
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			FInputModeGameOnly InputMode;

			PlayerController->SetInputMode(InputMode);
		}
		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
		FName LevelName(*CurrentLevelName);
		UGameplayStatics::OpenLevel(this, LevelName);
	}
	break;

	case EnumGameOver::Quit:
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		playerController->ConsoleCommand("quit");
	}
	break;

	case EnumGameOver::BackMainMenu:
	{
		FName LevelName(*PestilenceHUD->MainMenuSceneName);
		UGameplayStatics::OpenLevel(this, LevelName);
	}
	break;

	default:
		ConfirmationNo();
		break;
	}
}

void UPestilenceGameOverUserWidget::ConfirmationNo()
{
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	LoadGamePanel->SetVisibility(ESlateVisibility::Hidden);
	GameOverPanel->SetVisibility(ESlateVisibility::Visible); 
	
	LastCheckGameButton->SetFocus();
}

TArray<UWidget*> UPestilenceGameOverUserWidget::GetAllWidgetsRecursively()
{
	TArray<UWidget*> Children;
	this->WidgetTree->GetAllWidgets(Children);
	return(Children);
}

void UPestilenceGameOverUserWidget::Click(UUserWidget* Button)
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

