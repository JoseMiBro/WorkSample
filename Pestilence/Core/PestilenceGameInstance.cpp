#include "Pestilence/Core/PestilenceGameInstance.h"
#include "PestilenceSaveGameSubsystem.h"
#include "Blueprint/UserWidget.h"
#include <MoviePlayer/Public/MoviePlayer.h>

void UPestilenceGameInstance::Init()
{
	Super::Init();

	if (LoadingScreenClass)
	{
		LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass);
	}

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UPestilenceGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPestilenceGameInstance::EndLoadingScreen);
}

void UPestilenceGameInstance::LoadComplete
(
	const float LoadTime,
	const FString& MapName
) 
{
	if (bLoadGame)
	{
		AddLoadingScreen();

		UPestilenceSaveGameSubsystem* CUrrentSS = GetSubsystem<UPestilenceSaveGameSubsystem>();

		CUrrentSS->QuickLoad();
		//CUrrentSS->ResetSlotName();	
		SetLoadGame(false);
	}
	else 
	{
		RemoveLoadingScreen();
	}
}

void UPestilenceGameInstance::AddLoadingScreen()
{
	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->AddToViewport(999);
	}
}

void UPestilenceGameInstance::RemoveLoadingScreen()
{
	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->RemoveFromParent();
	}
}

void UPestilenceGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;

		LoadingScreen.MinimumLoadingScreenDisplayTime = 1.f;
		LoadingScreen.WidgetLoadingScreen = CreateWidget(this, LoadingScreenClass)->TakeWidget();
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);		
	}
}

void UPestilenceGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
  RemoveLoadingScreen();
}
