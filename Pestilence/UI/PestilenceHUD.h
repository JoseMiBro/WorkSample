#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PestilenceHUD.generated.h"

class UUserWidget;
class UPestilenceConversationUserWidget;
class UPestilenceMainMenuUserWidget;
class UPestilencePauseMenuUserWidget;
class UPestilenceGameOverUserWidget;
class UPestilenceDiaryUserWidget;
class UPlayerHUDWidget;
class UPestilenceOptionsMenu;
class UPestilenceConversationUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetsCreated);

UCLASS()
class PESTILENCE_API APestilenceHUD : public AHUD
{
	GENERATED_BODY()
public:

	APestilenceHUD();
	
	UPROPERTY()
		FOnWidgetsCreated OnWidgetsCreated;

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
		UPestilenceConversationUserWidget* ConversationWidgetInstance = nullptr;

	UPROPERTY()
		UPestilenceOptionsMenu* OptionsWidgetInstance = nullptr;

	UPROPERTY(BlueprintReadWrite)
		UUserWidget* HUDWidgetInstance = nullptr;

	UPROPERTY()
		UPlayerHUDWidget* PlayerHUDWidgetInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
		FString MainMenuSceneName = "MainMenuGym";


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
		FString StartNewGameSceneName = "Playground";

private:
	//static APestilenceHubManager* m_Instance;

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

	UPROPERTY(EditAnywhere, Category = "OptionsMenu")
		TSubclassOf<UPestilenceOptionsMenu> OptionsWidgetClass;

		UPROPERTY(EditAnywhere, Category = "OptionsMenu")
		TSubclassOf<UPestilenceConversationUserWidget> ConversationWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;

//protected:
//	// Sets default values for this actor's properties
//	APestilenceHubManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//UFUNCTION(BlueprintCallable, Category = "HubManager")
	//	static APestilenceHubManager* GetInstance();

	UFUNCTION(BlueprintCallable, Category = "HubManager")
		void ShowWidget(UUserWidget* _userWidget);

	//No puedo overlodear xq es ufunction :(
	UFUNCTION(BlueprintCallable, Category = "HubManager")
		void ShowWidgetZ(UUserWidget* _userWidget, int ZOrder);

	UFUNCTION(BlueprintCallable, Category = "HubManager")
		void HideWidget(UUserWidget* _userWidget);

	UFUNCTION(BlueprintImplementableEvent)
		void EnteredMenuLevel();

	UFUNCTION(BlueprintCallable, Category = "HubManager")
		void ShowMenu();
};
