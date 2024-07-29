// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PestilenceHubManager.generated.h"

class UUserWidget;
class UPestilenceConversationUserWidget;
class UPestilenceMainMenuUserWidget;
class UPestilencePauseMenuUserWidget;
class UPestilenceGameOverUserWidget;
class UPestilenceDiaryUserWidget;
class UPlayerHUDWidget;

UCLASS()
class PESTILENCE_API APestilenceHubManager : public AActor
{
	GENERATED_BODY()
		
public:
	UPROPERTY()
		UPestilenceConversationUserWidget* DialogueWidgetInstance = nullptr;

	UPROPERTY()
		UPestilenceDiaryUserWidget* DiaryMenuWidgetInstance = nullptr;

	UPROPERTY()
		UPestilenceMainMenuUserWidget* MainMenuWidgetInstance = nullptr;

	UPROPERTY()
		UPestilencePauseMenuUserWidget* PauseMenuWidgetInstance = nullptr;

	UPROPERTY()
		UPestilenceGameOverUserWidget* GameOverWidgetInstance = nullptr;

	UPROPERTY()
		UUserWidget* HUDWidgetInstance = nullptr;

	UPROPERTY()
		UPlayerHUDWidget* PlayerHUDWidgetInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
		FString MainMenuSceneName = "MainMenuGym";


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
		FString StartNewGameSceneName = "Playground";

private:
		static APestilenceHubManager* m_Instance;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
		TSubclassOf<UPestilenceConversationUserWidget> DialogueWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Feedback")
		TSubclassOf<UPestilenceDiaryUserWidget> DiaryMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "MainMenu")
		TSubclassOf<UPestilenceMainMenuUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "PauseMenu")
		TSubclassOf<UPestilencePauseMenuUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "GameOverMenu")
		TSubclassOf<UPestilenceGameOverUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;
	
protected:	
	// Sets default values for this actor's properties
	APestilenceHubManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "HubManager")
		static APestilenceHubManager* GetInstance();

	UFUNCTION(BlueprintCallable, Category = "HubManager")
		void InViewport(UUserWidget* _userWidget);

	UFUNCTION(BlueprintCallable, Category = "HubManager")
		void OutViewport(UUserWidget* _userWidget);
};
