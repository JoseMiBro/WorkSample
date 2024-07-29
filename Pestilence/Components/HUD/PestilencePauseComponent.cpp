// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Components/HUD/PestilencePauseComponent.h"

#include "Pestilence/UI/PestilenceHubManager.h"
#include "Pestilence/UI/PestilencePauseMenuUserWidget.h"
#include "Pestilence/UI/PestilenceGameOverUserWidget.h"
#include "Pestilence/UI/PestilenceDiaryUserWidget.h"
#include "Pestilence/UI/Options/PestilenceOptionsMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/UI/PestilenceHUD.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/Engine.h"


// Sets default values for this component's properties
UPestilencePauseComponent::UPestilencePauseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPestilencePauseComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController && PlayerController->GetHUD())
		{
			PestilenceHUD = Cast<APestilenceHUD>(PlayerController->GetHUD());
		}
	}
}


// Called every frame
void UPestilencePauseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPestilencePauseComponent::AddPauseMenu()
{
	if (PestilenceHUD)
	{
		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
		if (PestilenceHUD->MainMenuSceneName == CurrentLevelName ||
			PestilenceHUD->GameOverWidgetInstance->IsInViewport() ||
			PestilenceHUD->DiaryMenuWidgetInstance->IsInViewport() ||
			PestilenceHUD->OptionsWidgetInstance->IsInViewport() ||
			bIsDiaryOpen)
		{
			return;
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetOwner(), 0);

		if (PlayerController)
		{
			if (bIsPauseOpen)
			{
				PestilenceHUD->PauseMenuWidgetInstance->ContinueGame();
				return;
			}

			if (!GetWorld()->IsPaused())
			{
				bIsPauseOpen = true;

				PlayerController->bShowMouseCursor = true;

				FInputModeGameAndUI InputMode;
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				InputMode.SetHideCursorDuringCapture(false);

				PlayerController->SetInputMode(InputMode);
				PlayerController->SetPause(true);

				PestilenceHUD->ShowWidgetZ(PestilenceHUD->PauseMenuWidgetInstance, 99);
				PestilenceHUD->PauseMenuWidgetInstance->ContinueGameButton->SetUserFocus(PlayerController);
			}
		}
	}
}

void UPestilencePauseComponent::AddDiaryMenu()
{
	if (PestilenceHUD)
	{
		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
		if (PestilenceHUD->MainMenuSceneName == CurrentLevelName ||
			PestilenceHUD->GameOverWidgetInstance->IsInViewport() ||
			PestilenceHUD->PauseMenuWidgetInstance->IsInViewport() ||
			PestilenceHUD->OptionsWidgetInstance->IsInViewport() ||
			bIsPauseOpen)
		{
			return;
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			if (bIsDiaryOpen)
			{
				bIsDiaryOpen = false;
				FInputModeGameOnly InputMode;

				PlayerController->SetInputMode(InputMode);
				//
				PlayerController->bShowMouseCursor = false;
				PlayerController->SetPause(false);

				PestilenceHUD->HideWidget(PestilenceHUD->DiaryMenuWidgetInstance);
				UGameplayStatics::PlaySound2D(this, PestilenceHUD->DiaryMenuWidgetInstance->CloseDiarySound);
				return;
			}

			if (!GetWorld()->IsPaused())
			{
				bIsDiaryOpen = true;

				PlayerController->bShowMouseCursor = true;

				FInputModeGameAndUI InputMode;
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				InputMode.SetHideCursorDuringCapture(false);

				PlayerController->SetInputMode(InputMode);
				PlayerController->SetPause(true);

				PestilenceHUD->ShowWidget(PestilenceHUD->DiaryMenuWidgetInstance);
				UGameplayStatics::PlaySound2D(this, PestilenceHUD->DiaryMenuWidgetInstance->OpenDiarySound);
			}
		}
	}
}

void UPestilencePauseComponent::AddGameOverMenu()
{
	if (PestilenceHUD)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetOwner(), 0);

		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true;

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);

			PlayerController->SetInputMode(InputMode);
			PlayerController->DisableInput(PlayerController);

			PlayerController->SetPause(true);

			PestilenceHUD->ShowWidgetZ(PestilenceHUD->GameOverWidgetInstance, 99);

			PestilenceHUD->GameOverWidgetInstance->LastCheckGameButton->SetUserFocus(PlayerController);
			return;
		}
	}
}

bool UPestilencePauseComponent::IsGamePaused()
{
	UWorld* World = GEngine->GetWorld();
	if (World != nullptr)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode();
		if (GameMode != nullptr)
		{
			return GameMode->IsPaused();
		}
	}
	return false;
}

