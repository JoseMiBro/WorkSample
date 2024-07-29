#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../Props/PostProcessBlendZone.h"
#include "PestilenceGameInstance.generated.h"

class UUserWidget;
class UMaterialParameterCollection;

UCLASS()
class PESTILENCE_API UPestilenceGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init() override;

	virtual void LoadComplete
	(
		const float LoadTime,
		const FString& MapName
	) override;

	
	UFUNCTION(BlueprintCallable)
	void CheckGameOver() { bIsGameOver = !bIsGameOver; }
	
	UFUNCTION(BlueprintCallable)
	void SetGameOver(bool _bIsGameOver) { bIsGameOver = !bIsGameOver; }
	
	UFUNCTION(BlueprintCallable)
	void ConsumeLoadGame() { bLoadGame = !bLoadGame; }
	
	UFUNCTION(BlueprintCallable)
	void SetLoadGame(bool _bLoadGame) { bLoadGame = _bLoadGame; }

	UFUNCTION(BlueprintCallable)
	void AddLoadingScreen();
	UFUNCTION(BlueprintCallable)
	void RemoveLoadingScreen();

	UPROPERTY(EditAnywhere, Category = "LoadingScreen")
	TSubclassOf<class UUserWidget> LoadingScreenClass;
	
	UPROPERTY(EditAnywhere, Category = "VideoWidgets")
	TSubclassOf<class UUserWidget> IntroVideoClass;

	UPROPERTY(BlueprintReadOnly)
	class UUserWidget* LoadingScreenWidget;

	UPROPERTY()
	FString CurrentSaveSlotName;

	UPROPERTY(EditAnywhere, Category = "Pestilence | PostProcessBlend")
	TObjectPtr<UMaterialParameterCollection> ToonShaderParameterCollection;
	
  UPROPERTY(BlueprintReadWrite, Category = "Pestilence | PostProcessBlend")
  TArray<FPostProcessBlendParameter> ParameterArray;

	UFUNCTION()
		virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
		virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	UPROPERTY(BlueprintReadOnly)
	bool bLoadGame = false;

protected:

	UPROPERTY()
	bool bIsGameOver = false;

	UPROPERTY()
	bool bFirstLoad = true;
};
