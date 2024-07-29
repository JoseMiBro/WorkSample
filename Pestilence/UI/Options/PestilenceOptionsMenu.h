// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Scalability.h"

#include "PestilenceOptionsMenu.generated.h"

class UCanvasPanel;
class UButton;
class UComboBoxString;
class USlider;
class UCheckBox;
class USoundClass;
class UTextBlock;
class UInputAction;
class UInputKeySelector;

class UInputMappingContext;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmWindowO);

UENUM(BlueprintType)
enum class EnumOptionGoMenu : uint8
{
	MainMenu					UMETA(DisplayName = "MainMenu"),
	Pause							UMETA(DisplayName = "Pause"),
	GameOver					UMETA(DisplayName = "GameOver")
}; 

UENUM(BlueprintType)
enum class EnumOption : uint8
{
	ConfirmationExit	UMETA(DisplayName = "ConfirmationExit"),
	Confirmation			UMETA(DisplayName = "Confirmation")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitOptions);

UCLASS()
class PESTILENCE_API UPestilenceOptionsMenu : public UUserWidget
{
	GENERATED_BODY()

public:
  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Movement")
	FOnExitOptions OnExitOptions;

	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Movement")
	FOnExitOptions OnOpenOptions;

	UPROPERTY(BlueprintAssignable)
	FOnConfirmWindowO OnConfirmWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UUserWidget*> SpecialMoveGamepad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpecialMoveGamepadCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInOptions = false;

	//8UFUNCTION(BlueprintCallable)
	//8void ChangeSpecialMoveGamepadFocus();

	UPROPERTY(EditAnywhere)
		USoundClass* Master;

	UPROPERTY(EditAnywhere)
		USoundClass* VFX;

	UPROPERTY(EditAnywhere)
		USoundClass* Voices;

	UPROPERTY(EditAnywhere)
		USoundClass* Music;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Mapping", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Mapping", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputMappingContext> InputMappingContext;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Mapping", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputMappingContext> PossessionInputMappingContext;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Mapping", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputMappingContext> LockpickInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> MoveAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> LockpickMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> CrouchAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> WalkAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> RollAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> InteractAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> LeanRightAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> LeanLeftAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> SaveAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> LoadAction;

		//Equal Interact
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> InteractUIAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> AbilityCastAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> AbilityPrepareAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> AbilitySelect1Action;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> AbilitySelect2Action;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> AbilitySelect3Action;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> AbilitySelect4Action;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> AbilitySelectNextAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> AbilitySelectPrevAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Actions", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> DiaryAction;

		int32 GeneralGraphicsQuality;
		int32 AAGraphicsQuality;
		int32 PPGraphicsQuality;
		int32 VFXGraphicsQuality;
		int32 TextureGraphicsQuality;
		int32 GlobalIlluminationQuality;
		int32 ShadowGraphicsQuality;
		int32 LevelDetailGraphicsQuality;
    int32 FidelityFXQuality;
    int32 ResolutionX;
    int32 ResolutionY;

		float PestilenceFOV;
		float FPS;
		bool bSubtitle;

	UPROPERTY(BlueprintReadWrite)
		EnumOptionGoMenu OptionGoMenu;

	UPROPERTY()
		EnumOption OptionMenu;

		UFUNCTION(BlueprintCallable)
		void SetOptionsFocusInit();

protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ConfimationText;

		UPROPERTY(EditAnywhere, Category = "OptionsMenu")
		FString ConfirmationSaveText = "Are you sure you want to SAVE CHANGES?";

		UPROPERTY(EditAnywhere, Category = "OptionsMenu")
		FString ConfirmationTextExit = "You have UNSAVED items, do you want to SAVE before EXIT?";

	//UInputKeySelector
	//Controls
	UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyForwardSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyLeftSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyBackSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyRightSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyCrouchSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyWalkSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyRollSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyInteractSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyLeanRightSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyLeanLeftSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeySaveSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyLoadSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyAbilityCastSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyAbilityPrepareSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyAbility1Selector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyAbility2Selector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyAbility3Selector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyAbility4Selector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyAbilityNextSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyAbilityPrevSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* KeyDiarySelector;

		//GamePad
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadCrouchSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadRollSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadInteractSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadLeanRightSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadLeanLeftSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadAbilityCastSelector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadAbilityPrepareSelector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadAbility1Selector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadAbility2Selector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadAbility3Selector;
		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadAbility4Selector;

		UPROPERTY(meta = (BindWidget))
		UInputKeySelector* PadDiarySelector;

	//Panels
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* OptionsMenuPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* GameplayOptionsMenuPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* GraphicsOptionsMenuPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* SoundOptionsMenuPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* ControlsOptionsMenuPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* ConfirmationPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* KeyBoardOptionsMenuPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* GamepadOptionsMenuPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* KeyControlsPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* PadControlsPanel;

	//ComboBox
	//Panel Graphics
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* ResolutionComboBox;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UComboBoxString* FidelityFXComboBox;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UComboBoxString* GeneralQualityComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* QualityComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* FrameRateComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* AntialiasingComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UComboBoxString* GlobalIlluminationComboBox; 
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UComboBoxString* ShadowComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* FullscreenComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* TextureComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* VFXComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* LevelDetailComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UComboBoxString* PostProccessComboBox;

	//Slider
  //Panel Gameplay
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    USlider* SensitivityPadXSlider;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    USlider* SensitivityPadYSlider;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    USlider* SensitivityKeyXSlider;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    USlider* SensitivityKeyYSlider;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    USlider* AccelerationPadXSlider;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    USlider* AccelerationPadYSlider;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    USlider* AccelerationKeyXSlider;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    USlider* AccelerationKeyYSlider;

		//Panel Graphics
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* ScaleResolutionSlider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* DistanceResolutionSlider;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    USlider* FidelityFXSharpnessSlider;

	//Panel Volume
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* GeneralVolumeSlider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* VFXVolumeSlider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* VoicesVolumeSlider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* MusicVolumeSlider;

	//Check Box
	//Panel Gameplay
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* PadXAxisCheckBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* PadYAxisCheckBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* KeyXAxisCheckBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* KeyYAxisCheckBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* PadVibrationCheckBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* HeadBobbingCheckBox;

	//Panel Volume
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCheckBox* SubtitlesCheckBox;

	//Button
	//Panel Options
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* OptionsGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* OptionsVideoButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* OptionsSoundButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* OptionsReturnButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ConfirmationButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* DefaultButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ConfirmationYesButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ConfirmationNoButton;

	//Panel Gameplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* KeyOptionsControlsButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* PadOptionsControlsButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* PadToGameplayButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* KeyToGameplayButton;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* KeyOptionsReturnButton;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* PadOptionsReturnButton;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* KeyControlsButton;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* PadControlsButton;

	//Panel Controls
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUserWidget* ControlReturnGameplayOptionsButton;

private:
	FTimerHandle TimerHandle;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void Click(UUserWidget* Button);
	UFUNCTION(BlueprintCallable)
	TArray<UWidget*> GetAllWidgetsRecursively();

private:
	void AppliedChanges();
	void AppliedControlsChanges();
	void SetKeyInputs();
	void SetDefaultInputs();
	void UnMapContext(UInputMappingContext* _InputMappingContext);
	void GameplayConfig(bool InvertKeyX = false, bool InvertKeyY = false, bool InvertPadX = false, bool InvertPadY = false, float SensitivityKeyX = 0.7f, float SensitivityKeyY = 0.7f, 
		float SensitivityPadX = 0.7f, float SensitivityPadY = 0.7f,  float AccelerationKeyX = 0.f, float AccelerationKeyY = 0.f, float AccelerationPadX = 0.5f, float AccelerationPadY = 0.5f, 
		bool bVibration = true, bool bHeadBobbing = true);

	void OnComboQualityComboBoxSelectionChanged(Scalability::FQualityLevels Quality);
	void OnComboAntialiasingComboBoxSelectionChanged(int32 SelectedItem);
	void OnComboGlobalIlluminationComboBoxSelectionChanged(int32 SelectedItem);
	void OnComboShadowComboBoxSelectionChanged(int32 SelectedItem);
	void OnComboTextureComboBoxSelectionChanged(int32 SelectedItem);
	void OnComboVFXSelectionChanged(int32 SelectedItem);
	void OnComboLevelDetailSelectionChanged(int32 SelectedItem);
	void OnComboPostProccessSelectionChanged(int32 SelectedItem);


	UFUNCTION()
		void Options();

	UFUNCTION()
		void OptionsKey();

	UFUNCTION()
		void OptionsPad();

	UFUNCTION()
		void OptionsKeyControl();

	UFUNCTION()
		void OptionsPadControl();

	UFUNCTION()
		void OptionsVideo();

	UFUNCTION()
		void OptionsSound();

	//Exit
	UFUNCTION()
		void CloseOptions();
		void Exit();

	UFUNCTION()
		void OptionsControls();

	UFUNCTION()
		void OptionsConfirmation();

	UFUNCTION()
		void OptionsDefault();

	UFUNCTION()
		void OptionsConfirmationYes();

	UFUNCTION()
		void OptionsConfirmationNo();
	
  //Panel Graphics
  UFUNCTION()
  void OnComboFidelityFXSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
  void SetFidelityFXQuality(int32 _FidelityFXQuality);

  UFUNCTION()
  void OnSliderFidelityFXSharpnessChanged(float Value);

  UFUNCTION()
    void OnComboResolutionSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
		void SetResolution(FString SelectedItem);
		void SetComboBoxResolution();

	UFUNCTION()
		void OnSliderResolutionScaleChanged(float Value);

	UFUNCTION()
		void OnSliderDistanceResolutionChanged(float Value);

	UFUNCTION()
		void OnComboQualityComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void OnComboQualityCustomComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void OnComboFrameRateComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void OnComboAntialiasingComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
    void OnComboGlobalIlluminationComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

  UFUNCTION()
    void OnComboShadowComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void OnComboFulscreenComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void OnComboTextureComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void OnComboVFXSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void OnComboLevelDetailSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void OnComboPostProccessSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	//Panel Volume
	UFUNCTION()
	 void OnSliderVolumeMarterChanged(float Value);

	UFUNCTION()
	 void OnSliderVolumeVFXChanged(float Value);

	UFUNCTION()
	 void OnSliderVolumeVoicesChanged(float Value);

	UFUNCTION()
	 void OnSliderVolumeMusicChanged(float Value);

	UFUNCTION()
	 void OnCheckBoxSubtitlesChanged(bool IsActivated);

	UFUNCTION()
	 void OnCheckBoxXKeyChanged(bool IsActivated);
	 
	UFUNCTION()
	 void OnCheckBoxYKeyChanged(bool IsActivated);
	 
	UFUNCTION()
	 void OnCheckBoxXPadChanged(bool IsActivated);
	 
	UFUNCTION()
	 void OnCheckBoxYPadChanged(bool IsActivated);

	UFUNCTION()
	 void OnCheckHeadBobbingChanged(bool IsActivated);
	
	UFUNCTION()
	 void OnSliderSensitivityPadXChanged(float Value);

	UFUNCTION()
	 void OnSliderSensitivityPadYChanged(float Value);

	UFUNCTION()
	 void OnSliderSensitivityKeyXChanged(float Value);

	UFUNCTION()
	 void OnSliderSensitivityKeyYChanged(float Value);
	
	UFUNCTION()
	 void OnSliderAccelerationPadXChanged(float Value);

	UFUNCTION()
	 void OnSliderAccelerationPadYChanged(float Value);

	UFUNCTION()
	 void OnSliderAccelerationKeyXChanged(float Value);

	UFUNCTION()
	 void OnSliderAccelerationKeyYChanged(float Value);

	UFUNCTION()
	 void OnCheckBoxPadVibrationChanged(bool IsActivated);

	 //InputSelector
	UFUNCTION()
	void OnSelectorKeyForwardChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyBackChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyRightChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyLeftChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyCrouchChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyWalkChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyRollChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyInteractChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyLeanRightChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyLeanLeftChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeySaveChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyLoadChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyAbilityCastChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyAbilityPrepareChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyAbility1Change(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyAbility2Change(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyAbility3Change(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyAbility4Change(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyAbilitySelectNextChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyAbilitySelectPrevChange(FInputChord SelectedKey);
	UFUNCTION()
	void OnSelectorKeyDiaryChange(FInputChord SelectedKey);

	UFUNCTION()
	void OnSelectorPadCrouchChange(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadRollChange(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadInteractChange(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadLeanRightChange(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadLeanLeftChange(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadAbilityCastChange(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadAbilityPrepareChange(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadAbility1Change(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadAbility2Change(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadAbility3Change(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadAbility4Change(FInputChord SelectedPad);
	UFUNCTION()
	void OnSelectorPadDiaryChange(FInputChord SelectedPad);
};
