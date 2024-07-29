// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilencePauseMenuUserWidget.h"

#include "Kismet/GameplayStatics.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

#include "Pestilence/Components/HUD/PestilencePauseComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/UI/PestilenceHUD.h"
#include "Pestilence/UI/Options/PestilenceOptionsMenu.h"

#include "Blueprint/WidgetTree.h"
#include "../Core/PestilenceGameInstance.h"
#include "../Core/PestilenceSaveGameSubsystem.h"

void UPestilencePauseMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmationNo();
}

void UPestilencePauseMenuUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> Children;
	ContinueGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::ContinueGame);
		}
	}
	Children.Empty();

	LastCheckGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::LastCheckGame);
		}
	}
	Children.Empty();

	SaveGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::SaveGame);
		}
	}
	Children.Empty();

	MainMenuGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::BackMainMenu);
		}
	}
	Children.Empty();

	ReloadGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::ReloadGame);
		}
	}
	Children.Empty();

	OptionsButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::Options);
		}
	}
	Children.Empty();

	ExitButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::Exit);
		}
	}
	Children.Empty();

	ConfirmationYesButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::ConfirmationYes);
		}
	}
	Children.Empty();

	ConfirmationNoButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::ConfirmationNo);
		}
	}
	Children.Empty();

	BackPauseMenuButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::ConfirmationNo);
		}
	}
	Children.Empty();

	BackPauseMenuLoadButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::ConfirmationNo);
		}
	}
	Children.Empty();

	LoadSaveGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::LoadSaveGame);
		}
	}
	Children.Empty();

	Slot_1Button->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::SlotSave1);
		}
	}
	Children.Empty();

	Slot_2Button->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::SlotSave2);
		}
	}
	Children.Empty();

	Slot_3Button->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilencePauseMenuUserWidget::SlotSave3);
		}
	}
	Children.Empty();
}

bool UPestilencePauseMenuUserWidget::Initialize()
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

void UPestilencePauseMenuUserWidget::ContinueGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		APestilenceCharacter* PestilenceCharacter = Cast<APestilenceCharacter>(PlayerController->GetPawn());
		if (PestilenceCharacter) 
		{
			PestilenceCharacter->GetPauseComponent()->bIsPauseOpen = false;

			FInputModeGameOnly InputMode;

			PlayerController->SetInputMode(InputMode);
			//
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetPause(false);

			binViewport = false;

			LoadGamePanel->SetVisibility(ESlateVisibility::Hidden);
			PauseMenuPanel->SetVisibility(ESlateVisibility::Visible);
			SaveGamePanel->SetVisibility(ESlateVisibility::Hidden);
			ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);

			OnContinueGame.Broadcast();

			RemoveFromParent();
		}
	}
}

void UPestilencePauseMenuUserWidget::Click(UUserWidget* Button)
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

TArray<UWidget*> UPestilencePauseMenuUserWidget::GetAllWidgetsRecursively()
{
	TArray<UWidget*> Children;
	this->WidgetTree->GetAllWidgets(Children);
	return(Children);
}

void UPestilencePauseMenuUserWidget::LastCheckGame()
{
	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//if (PlayerController)
	//{
	//	FInputModeGameOnly InputMode;

	//	PlayerController->SetInputMode(InputMode);
	//}
	//FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	//FName LevelName(*CurrentLevelName);
	//UGameplayStatics::OpenLevel(this, LevelName);
}

void UPestilencePauseMenuUserWidget::SaveGame()
{
	LoadGamePanel->SetVisibility(ESlateVisibility::Hidden);
	PauseMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SaveGamePanel->SetVisibility(ESlateVisibility::Visible);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	
	Slot_1Button->SetFocus();
}

void UPestilencePauseMenuUserWidget::LoadSaveGame()
{
	LoadGamePanel->SetVisibility(ESlateVisibility::Visible);
	PauseMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SaveGamePanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);

	Slot_1LoadButton->SetFocus();
}

void UPestilencePauseMenuUserWidget::BackMainMenu()
{
	SetConfirmationPanel(ConfirmationBackMainMenu, EnumPause::BackMainMenu);
}

void UPestilencePauseMenuUserWidget::ReloadGame()
{
	SetConfirmationPanel(ConfirmationRestart, EnumPause::Restart);
}

void UPestilencePauseMenuUserWidget::Options()
{
	if (PestilenceHUD)
	{
		PestilenceHUD->ShowWidgetZ(PestilenceHUD->OptionsWidgetInstance, 99);
		PestilenceHUD->OptionsWidgetInstance->SetOptionsFocusInit();
		PestilenceHUD->OptionsWidgetInstance->OptionGoMenu = EnumOptionGoMenu::Pause;
		RemoveFromParent();

	}
}

void UPestilencePauseMenuUserWidget::Exit()
{
	SetConfirmationPanel(ConfirmationExit, EnumPause::Quit);
}

void UPestilencePauseMenuUserWidget::SetConfirmationPanel(FString _ConfirmationText, EnumPause _EnumPause) 
{
	ConfirmationPanel->SetVisibility(ESlateVisibility::Visible);

	ConfirmationNoButton->SetFocus();
	ConfimationText->SetText(FText::FromString(_ConfirmationText));
	enumPause = _EnumPause;
	OnConfirmWindow.Broadcast();
}

void UPestilencePauseMenuUserWidget::ConfirmationYes()
{
	switch (enumPause) 
	{
	case EnumPause::Restart: 
  {
    UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
    UPestilenceSaveGameSubsystem* SGSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
    SGSubsystem->SetCheckPointSlotName();
    UGameplayStatics::DeleteGameInSlot(SGSubsystem->GetCurrentSlotName(), 0);
    SGSubsystem->ResetParameters();

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

	case EnumPause::Quit: 
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		playerController->ConsoleCommand("quit");
	}
		break;

	case EnumPause::BackMainMenu: 
  {
    UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
    UPestilenceSaveGameSubsystem* SGSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
		SGSubsystem->ResetParameters();

		FName LevelName(*PestilenceHUD->MainMenuSceneName);
		UGameplayStatics::OpenLevel(this, LevelName);
	}
		break;

	default:
		ConfirmationNo();
		break;
	}
}

void UPestilencePauseMenuUserWidget::ConfirmationNo()
{
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UPestilencePauseMenuUserWidget::SlotSave1()
{
	SetConfirmationPanel(ConfirmationSlot1, EnumPause::FirstSlot);
}

void UPestilencePauseMenuUserWidget::SlotSave2()
{
	SetConfirmationPanel(ConfirmationSlot2, EnumPause::SecondSlot);
}

void UPestilencePauseMenuUserWidget::SlotSave3()
{
	SetConfirmationPanel(ConfirmationSlot3, EnumPause::ThirdSlot);
}
