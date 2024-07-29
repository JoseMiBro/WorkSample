// Fill out your copyright notice in the Description page of Project Settings.

#include "Pestilence/UI/PestilenceHubManager.h"

#include "Kismet/GameplayStatics.h"

#include "Pestilence/UI/PestilenceConversationUserWidget.h"
#include "Pestilence/UI/PestilenceMainMenuUserWidget.h"
#include "Pestilence/UI/PestilencePauseMenuUserWidget.h"
#include "Pestilence/UI/PestilenceGameOverUserWidget.h"
#include "Pestilence/UI/PestilenceDiaryUserWidget.h"
#include "Pestilence/UI/PlayerHUDWidget.h"

#include "Components/Button.h"
APestilenceHubManager* APestilenceHubManager::m_Instance = nullptr;

APestilenceHubManager* APestilenceHubManager::GetInstance()
{
	//if (m_Instance == nullptr)
	//{
	//	UWorld* World = GetWorld();
	//	if (!m_Instance)
	//	{
	//		m_Instance = World->SpawnActor<APestilenceHubManager>();
	//	}
	//}
	return m_Instance;
}

// Sets default values
APestilenceHubManager::APestilenceHubManager()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void APestilenceHubManager::BeginPlay()
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
				InViewport(PlayerHUDWidgetInstance);
			}
		}

		if (DiaryMenuWidgetClass)
		{
			DiaryMenuWidgetInstance = CreateWidget<UPestilenceDiaryUserWidget>(GetWorld(), DiaryMenuWidgetClass);
			if (DiaryMenuWidgetInstance)
			{
				OutViewport(DiaryMenuWidgetInstance);
			}
		}

		if (HUDWidgetClass)
		{
			HUDWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
			if (HUDWidgetInstance)
			{
				InViewport(HUDWidgetInstance);
			}
		}

		if (PauseMenuWidgetClass)
		{
			PauseMenuWidgetInstance = CreateWidget<UPestilencePauseMenuUserWidget>(GetWorld(), PauseMenuWidgetClass);
			if (PauseMenuWidgetInstance)
			{
				OutViewport(PauseMenuWidgetInstance);
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
	}

	else 
	{
		if (MainMenuWidgetClass)
		{
			MainMenuWidgetInstance = CreateWidget<UPestilenceMainMenuUserWidget>(GetWorld(), MainMenuWidgetClass);
			if (MainMenuWidgetInstance)
			{
				InViewport(MainMenuWidgetInstance);

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
	}
}

void APestilenceHubManager::InViewport(UUserWidget* _userWidget)
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

void APestilenceHubManager::OutViewport(UUserWidget* _userWidget)
{
	if (_userWidget)
	{
		if (_userWidget->IsInViewport())
		{
			_userWidget->RemoveFromParent();
		}
	}
}

