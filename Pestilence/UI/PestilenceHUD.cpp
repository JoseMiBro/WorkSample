#include "Pestilence/UI/PestilenceHUD.h"

#include "Kismet/GameplayStatics.h"

#include "Pestilence/UI/PestilenceConversationUserWidget.h"
#include "Pestilence/UI/PestilenceMainMenuUserWidget.h"
#include "Pestilence/UI/PestilencePauseMenuUserWidget.h"
#include "Pestilence/UI/PestilenceGameOverUserWidget.h"
#include "Pestilence/UI/PestilenceDiaryUserWidget.h"
#include "Pestilence/UI/PlayerHUDWidget.h"
#include "Pestilence/UI/Options/PestilenceOptionsMenu.h"
#include "Pestilence/UI/PestilenceConversationUserWidget.h"

APestilenceHUD::APestilenceHUD()
{
}

void APestilenceHUD::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	if (CurrentLevelName != MainMenuSceneName)
	{
		if (PlayerHUDWidgetClass)
		{
			PlayerHUDWidgetInstance = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
			if (PlayerHUDWidgetInstance)
			{
				ShowWidget(PlayerHUDWidgetInstance);
			}
		}

		if (DiaryMenuWidgetClass)
		{
			DiaryMenuWidgetInstance = CreateWidget<UPestilenceDiaryUserWidget>(GetWorld(), DiaryMenuWidgetClass);
			if (DiaryMenuWidgetInstance)
			{
				HideWidget(DiaryMenuWidgetInstance);
			}
		}

		if (HUDWidgetClass)
		{
			HUDWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
			if (HUDWidgetInstance)
			{
				ShowWidget(HUDWidgetInstance); //16-08 a peticion de diseño Show->Hide
			}
		}

		if (PauseMenuWidgetClass)
		{
			PauseMenuWidgetInstance = CreateWidget<UPestilencePauseMenuUserWidget>(GetWorld(), PauseMenuWidgetClass);
			if (PauseMenuWidgetInstance)
			{
				HideWidget(PauseMenuWidgetInstance);
			}
		}

		if (GameOverWidgetClass)
		{
			GameOverWidgetInstance = CreateWidget<UPestilenceGameOverUserWidget>(GetWorld(), GameOverWidgetClass);
			if (GameOverWidgetInstance)
			{
				//OutViewport(GameOverWidgetInstance);
			}
		}

		if (ConversationWidgetClass)
		{
			ConversationWidgetInstance = CreateWidget<UPestilenceConversationUserWidget>(GetWorld(), ConversationWidgetClass);
			if (ConversationWidgetInstance)
			{
				ShowWidget(ConversationWidgetInstance);
				ConversationWidgetInstance->DisableAll(nullptr);
			}
		}
	}

	else
	{
		if (MainMenuWidgetClass)
		{
			EnteredMenuLevel();
			
		}
	}

	if (OptionsWidgetClass)
	{
		OptionsWidgetInstance = CreateWidget<UPestilenceOptionsMenu>(GetWorld(), OptionsWidgetClass);
		if (OptionsWidgetInstance)
		{
			HideWidget(OptionsWidgetInstance);
		}
	}

	OnWidgetsCreated.Broadcast();
}

void APestilenceHUD::ShowWidget(UUserWidget* _userWidget)
{
	if (_userWidget)
	{
		if (!_userWidget->IsInViewport())
		{
			_userWidget->AddToViewport(1);
		}

		else
		{
			_userWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APestilenceHUD::ShowWidgetZ(UUserWidget* _userWidget, int ZOrder)
{
	if (_userWidget)
	{
		if (!_userWidget->IsInViewport())
		{
			_userWidget->AddToViewport(ZOrder);
		}

		else
		{
			_userWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APestilenceHUD::HideWidget(UUserWidget* _userWidget)
{
	if (_userWidget)
	{
		if (_userWidget->IsInViewport())
		{
			_userWidget->RemoveFromParent();
		}
	}
}

void APestilenceHUD::ShowMenu()
{
	MainMenuWidgetInstance = CreateWidget<UPestilenceMainMenuUserWidget>(GetWorld(), MainMenuWidgetClass);
	if (MainMenuWidgetInstance)
	{
		ShowWidget(MainMenuWidgetInstance);

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true;

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);

			PlayerController->SetInputMode(InputMode);
		}
	}
}
