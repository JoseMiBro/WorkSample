// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/Options/PestilenceOptionsMenu.h"

#include "Engine.h"
#include "Scalability.h"
#include "Engine/GameViewportClient.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/IntPoint.h"
#include "GameFramework/GameUserSettings.h"
#include "Slate/SceneViewport.h"
#include "Kismet/GameplayStatics.h"

#include "Sound/SoundClass.h"
#include "Camera/PlayerCameraManager.h"
#include "TimerManager.h"

#include "GameFramework/InputSettings.h"
#include "InputCoreTypes.h" 
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "InputTriggers.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

#include "Pestilence/Characters/Input/PestilenceInputModifier.h"
#include "Pestilence/Core/PestilenceGameUserSettings.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/InputKeySelector.h"

#include "Pestilence/UI/PestilenceHUD.h"
#include "Pestilence/UI/PestilenceMainMenuUserWidget.h"
#include "Pestilence/UI/PestilencePauseMenuUserWidget.h"
#include "Pestilence/UI/PestilenceGameOverUserWidget.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"

void UPestilenceOptionsMenu::SetOptionsFocusInit()
{
	TArray<UWidget*> Children;

	OptionsGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->SetFocus();
		}
	}
	Children.Empty();
}

void UPestilenceOptionsMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPestilenceOptionsMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//Panel Options
	TArray<UWidget*> Children;

	OptionsGameButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::Options);
		}
	}
	Children.Empty();

	OptionsVideoButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsVideo);
		}
	}
	Children.Empty();

	OptionsSoundButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsSound);
		}
	}
	Children.Empty();

	ConfirmationButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsConfirmation);
		}
	}
	Children.Empty();

	ConfirmationYesButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsConfirmationYes);
		}
	}
	Children.Empty();

	ConfirmationNoButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsConfirmationNo);
		}
	}
	Children.Empty();

	DefaultButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsDefault);
		}
	}
	Children.Empty();

	//Exit
	OptionsReturnButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::CloseOptions);
		}
	}
	Children.Empty();

	//Panel Gameplay
	KeyOptionsControlsButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsKey);
		}
	}
	Children.Empty();

	PadOptionsControlsButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsPad);
		}
	}
	Children.Empty();

	PadToGameplayButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::Options);
		}
	}
	Children.Empty();

	KeyToGameplayButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::Options);
		}
	}
	Children.Empty();

	KeyOptionsReturnButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsKey);
		}
	}
	Children.Empty();

	PadOptionsReturnButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsPad);
		}
	}
	Children.Empty();

	KeyControlsButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsKeyControl);
		}
	}
	Children.Empty();

	PadControlsButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::OptionsPadControl);
		}
	}
	Children.Empty();

	PadXAxisCheckBox->OnCheckStateChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnCheckBoxXPadChanged);
	PadYAxisCheckBox->OnCheckStateChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnCheckBoxYPadChanged);
	KeyXAxisCheckBox->OnCheckStateChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnCheckBoxXKeyChanged);
	KeyYAxisCheckBox->OnCheckStateChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnCheckBoxYKeyChanged);

  SensitivityPadXSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderSensitivityPadXChanged);
  SensitivityPadYSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderSensitivityPadYChanged);
  SensitivityKeyXSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderSensitivityKeyXChanged);
  SensitivityKeyYSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderSensitivityKeyYChanged);

  AccelerationPadXSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderAccelerationPadXChanged);
	AccelerationPadYSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderAccelerationPadYChanged);
	AccelerationKeyXSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderAccelerationKeyXChanged);
	AccelerationKeyYSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderAccelerationKeyYChanged);

	PadVibrationCheckBox->OnCheckStateChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnCheckBoxPadVibrationChanged);

	//ControlKey
	KeyForwardSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyForwardChange);
	KeyLeftSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyLeftChange);
	KeyBackSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyBackChange);
	KeyRightSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyRightChange);

	KeyCrouchSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyCrouchChange);
	KeyWalkSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyWalkChange);
	KeyRollSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyRollChange);

	KeyInteractSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyInteractChange);

	KeyLeanRightSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyLeanRightChange);
	KeyLeanLeftSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyLeanLeftChange);

	KeySaveSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeySaveChange);
	KeyLoadSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyLoadChange);

	KeyAbilityCastSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyAbilityCastChange);
	KeyAbilityPrepareSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyAbilityPrepareChange);

	KeyAbility1Selector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyAbility1Change);
	KeyAbility2Selector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyAbility2Change);
	KeyAbility3Selector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyAbility3Change);
	KeyAbility4Selector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyAbility4Change);

	KeyAbilityNextSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyAbilitySelectNextChange);
	KeyAbilityPrevSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyAbilitySelectPrevChange);

	KeyDiarySelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorKeyDiaryChange);

	//ControlPad
	PadCrouchSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadCrouchChange);
	PadRollSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadRollChange);

	PadInteractSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadInteractChange);

	PadLeanRightSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadLeanRightChange);
	PadLeanLeftSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadLeanLeftChange);

	PadAbilityCastSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadAbilityCastChange);
	PadAbilityPrepareSelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadAbilityPrepareChange);

	PadAbility1Selector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadAbility1Change);
	PadAbility2Selector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadAbility2Change);
	PadAbility3Selector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadAbility3Change);
	PadAbility4Selector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadAbility4Change);

	PadDiarySelector->OnKeySelected.AddDynamic(this, &UPestilenceOptionsMenu::OnSelectorPadDiaryChange);

	//Panel Graphics
	GeneralQualityComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboQualityComboBoxSelectionChanged);
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboResolutionSelectionChanged);
	FrameRateComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboFrameRateComboBoxSelectionChanged);
	FullscreenComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboFulscreenComboBoxSelectionChanged);
	DistanceResolutionSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderDistanceResolutionChanged);
	ScaleResolutionSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderResolutionScaleChanged);
	FidelityFXSharpnessSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderFidelityFXSharpnessChanged);

	//Panel Graphics Custom
	QualityComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboQualityCustomComboBoxSelectionChanged);
	AntialiasingComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboAntialiasingComboBoxSelectionChanged);
	GlobalIlluminationComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboGlobalIlluminationComboBoxSelectionChanged);
	ShadowComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboShadowComboBoxSelectionChanged);
	TextureComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboTextureComboBoxSelectionChanged);
	VFXComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboVFXSelectionChanged);
	LevelDetailComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboLevelDetailSelectionChanged);
  PostProccessComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboPostProccessSelectionChanged);
  FidelityFXComboBox->OnSelectionChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnComboFidelityFXSelectionChanged);

	//Panel Volume
	GeneralVolumeSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderVolumeMarterChanged);
	VFXVolumeSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderVolumeVFXChanged);
	VoicesVolumeSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderVolumeVoicesChanged);
	MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnSliderVolumeMusicChanged);
	SubtitlesCheckBox->OnCheckStateChanged.AddDynamic(this, &UPestilenceOptionsMenu::OnCheckBoxSubtitlesChanged);

	//Panel Controls
	ControlReturnGameplayOptionsButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->OnClicked.AddDynamic(this, &UPestilenceOptionsMenu::Options);
		}
	}
	Children.Empty();
}

bool UPestilenceOptionsMenu::Initialize()
{
	bool parentSuccess = Super::Initialize();
	if (!parentSuccess) { return parentSuccess; }

	SetComboBoxResolution();

	AAGraphicsQuality = Scalability::GetQualityLevels().AntiAliasingQuality;
	PPGraphicsQuality = Scalability::GetQualityLevels().PostProcessQuality;
	VFXGraphicsQuality = Scalability::GetQualityLevels().EffectsQuality;
	TextureGraphicsQuality = Scalability::GetQualityLevels().TextureQuality;
	GlobalIlluminationQuality = Scalability::GetQualityLevels().GlobalIlluminationQuality;
	ShadowGraphicsQuality = Scalability::GetQualityLevels().ShadowQuality;
	LevelDetailGraphicsQuality = Scalability::GetQualityLevels().ViewDistanceQuality;

	AppliedChanges();

	Options();

	return parentSuccess;
}

void UPestilenceOptionsMenu::AppliedChanges()
{
	OnComboQualityComboBoxSelectionChanged(Scalability::GetQualityLevels());

	bool bHaveSave = false;

	GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bHaveSave"), bHaveSave, GGameIni);

	if (bHaveSave) { SetKeyInputs(); }

	else { SetDefaultInputs(); }

	AppliedControlsChanges();

	SubtitlesCheckBox->SetIsChecked(UGameplayStatics::AreSubtitlesEnabled());
	bSubtitle = UGameplayStatics::AreSubtitlesEnabled();

	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		float AuxResolutionNormalize;
		float AuxResolution;
		float AuxResolutionMin;
		float AuxResolutionMax;

		GameUserSettings->GetResolutionScaleInformationEx(AuxResolutionNormalize, AuxResolution, AuxResolutionMin, AuxResolutionMax);
		ScaleResolutionSlider->SetValue(AuxResolution);

    GConfig->GetInt(TEXT("PestilenceGroups"), TEXT("ResolutionX"), ResolutionX, GGameIni);
    GConfig->GetInt(TEXT("PestilenceGroups"), TEXT("ResolutionY"), ResolutionY, GGameIni);
		if(ResolutionX == 0 || ResolutionY == 0)
		{
			FIntPoint DefaultResolution = GameUserSettings->GetDesktopResolution();

			ResolutionX = DefaultResolution.X; 
      ResolutionY = DefaultResolution.Y;

      GConfig->SetInt(TEXT("PestilenceGroups"), TEXT("ResolutionX"), ResolutionX, GGameIni);
      GConfig->SetInt(TEXT("PestilenceGroups"), TEXT("ResolutionY"), ResolutionY, GGameIni);
		}
		FString AuxString = FString::Printf(TEXT("%dx%d"), ResolutionX, ResolutionY);

		ResolutionComboBox->SetSelectedOption(AuxString);

		if (GameUserSettings->GetFullscreenMode() == EWindowMode::Fullscreen)
		{
			FullscreenComboBox->SetSelectedOption(TEXT("Fullscreen"));
		}

		if (GameUserSettings->GetFullscreenMode() == EWindowMode::Windowed)
		{
			FullscreenComboBox->SetSelectedOption(TEXT("Window"));
		}

		if (GameUserSettings->GetFullscreenMode() == EWindowMode::WindowedFullscreen)
		{
			FullscreenComboBox->SetSelectedOption(TEXT("Windowed Fullscreen"));
		}

		if (GameUserSettings->GetFrameRateLimit() == 0)
		{
			FrameRateComboBox->SetSelectedOption(TEXT("Unlimited"));
		}

		else
		{
			FString AuxFrame = FString::Printf(TEXT("%d"), FMath::FloorToInt(GameUserSettings->GetFrameRateLimit()));
			FrameRateComboBox->SetSelectedOption(AuxFrame);
		}
	}

	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (PlayerCameraManager)
	{
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceFOV"), PestilenceFOV, GGameIni);

		DistanceResolutionSlider->SetValue(PestilenceFOV);
	}

	if (IsValid(Master))
	{
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceMasterVolume"), Master->Properties.Volume, GGameIni);
		GeneralVolumeSlider->SetValue(Master->Properties.Volume);
	}
	if (IsValid(VFX))
	{
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceEffectsVolume"), VFX->Properties.Volume, GGameIni);
		VFXVolumeSlider->SetValue(VFX->Properties.Volume);
	}
	if (IsValid(Master))
	{
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceVoicesVolume"), Voices->Properties.Volume, GGameIni);
		VoicesVolumeSlider->SetValue(Voices->Properties.Volume);
	}
	if (IsValid(Music))
	{
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceMusicVolume"), Music->Properties.Volume, GGameIni);
		MusicVolumeSlider->SetValue(Music->Properties.Volume);
	}

  GConfig->GetInt(TEXT("PestilenceGroups"), TEXT("FidelityFXQuality"), FidelityFXQuality, GGameIni);
  SetFidelityFXQuality(FidelityFXQuality);
  float FidelityFXSharpness;
  GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("FidelityFXSharpness"), FidelityFXSharpness, GGameIni);
	FidelityFXSharpnessSlider->SetValue(FidelityFXSharpness);
}

void UPestilenceOptionsMenu::AppliedControlsChanges()
{
	bool bAuxInvertKeyX, bAuxInvertKeyY, bAuxInvertPadX, bAuxInvertPadY, bIsVibrationPad, bHeadBobbing;
	float AuxSensitivityKeyX, AuxSensitivityKeyY, AuxSensitivityPadX, AuxSensitivityPadY,
		AuxAccelerationKeyX, AuxAccelerationKeyY, AuxAccelerationPadX, AuxAccelerationPadY;

	GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertKeyX"), bAuxInvertKeyX, GGameIni);
	GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertKeyY"), bAuxInvertKeyY, GGameIni);
	GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertPadX"), bAuxInvertPadX, GGameIni);
  GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertPadY"), bAuxInvertPadY, GGameIni);
  GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityKeyX"), AuxSensitivityKeyX, GGameIni);
  GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityKeyY"), AuxSensitivityKeyY, GGameIni);
  GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityPadX"), AuxSensitivityPadX, GGameIni);
  GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityPadY"), AuxSensitivityPadY, GGameIni);
  GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelKeyX"), AuxAccelerationKeyX, GGameIni);
  GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelKeyY"), AuxAccelerationKeyY, GGameIni);
  GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelPadX"), AuxAccelerationPadX, GGameIni);
  GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelPadY"), AuxAccelerationPadY, GGameIni);
	
	GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bIsVibrationPad"), bIsVibrationPad, GGameIni);
	GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bHeadBobbing"), bHeadBobbing, GGameIni);

	GameplayConfig(bAuxInvertKeyX, bAuxInvertKeyY, bAuxInvertPadX, bAuxInvertPadY, AuxSensitivityKeyX, AuxSensitivityKeyY, AuxSensitivityPadX, AuxSensitivityPadY, 
		AuxAccelerationKeyX, AuxAccelerationKeyY, AuxAccelerationPadX, AuxAccelerationPadY, bIsVibrationPad, bHeadBobbing);
}

void UPestilenceOptionsMenu::SetKeyInputs()
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		{
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultForwardKey"), PestilenceGameUserSettings->KeyDefaultForward, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLeftKey"), PestilenceGameUserSettings->KeyDefaultLeft, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultBackKey"), PestilenceGameUserSettings->KeyDefaultBack, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultRightKey"), PestilenceGameUserSettings->KeyDefaultRight, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultCrouchKey"), PestilenceGameUserSettings->KeyDefaultCrouch, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultWalkKey"), PestilenceGameUserSettings->KeyDefaultWalk, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultRollKey"), PestilenceGameUserSettings->KeyDefaultRoll, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultInteractKey"), PestilenceGameUserSettings->KeyDefaultInteract, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLeanRightKey"), PestilenceGameUserSettings->KeyDefaultLeanRight, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLeanLeftKey"), PestilenceGameUserSettings->KeyDefaultLeanLeft, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultSaveKey"), PestilenceGameUserSettings->KeyDefaultSave, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLoadKey"), PestilenceGameUserSettings->KeyDefaultLoad, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilityCastKey"), PestilenceGameUserSettings->KeyDefaultAbilityCast, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilityPrepareKey"), PestilenceGameUserSettings->KeyDefaultAbilityPrepare, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility1Key"), PestilenceGameUserSettings->KeyDefaultAbility1, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility2Key"), PestilenceGameUserSettings->KeyDefaultAbility2, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility3Key"), PestilenceGameUserSettings->KeyDefaultAbility3, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility4Key"), PestilenceGameUserSettings->KeyDefaultAbility4, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilitySelectNextKey"), PestilenceGameUserSettings->KeyDefaultAbilitySelectNext, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilitySelectPrevKey"), PestilenceGameUserSettings->KeyDefaultAbilitySelectPrev, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultDiaryKey"), PestilenceGameUserSettings->KeyDefaultDiary, GGameIni);

			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultCrouchKey"), PestilenceGameUserSettings->PadDefaultCrouch, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultRollKey"), PestilenceGameUserSettings->PadDefaultRoll, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultInteractKey"), PestilenceGameUserSettings->PadDefaultInteract, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultLeanRightKey"), PestilenceGameUserSettings->PadDefaultLeanRight, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultLeanLeftKey"), PestilenceGameUserSettings->PadDefaultLeanLeft, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbilityCastKey"), PestilenceGameUserSettings->PadDefaultAbilityCast, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbilityPrepareKey"), PestilenceGameUserSettings->PadDefaultAbilityPrepare, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility1Key"), PestilenceGameUserSettings->PadDefaultAbility1, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility2Key"), PestilenceGameUserSettings->PadDefaultAbility2, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility3Key"), PestilenceGameUserSettings->PadDefaultAbility3, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility4Key"), PestilenceGameUserSettings->PadDefaultAbility4, GGameIni);
			GConfig->GetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultDiaryKey"), PestilenceGameUserSettings->PadDefaultDiary, GGameIni);
		}

		TArray<FEnhancedActionKeyMapping> Maps = InputMappingContext->GetMappings();

		UnMapContext(InputMappingContext);

		KeyRightSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultRight));
		OnSelectorKeyRightChange(FKey(*PestilenceGameUserSettings->KeyDefaultRight));
		KeyBackSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultBack));
		OnSelectorKeyBackChange(FKey(*PestilenceGameUserSettings->KeyDefaultBack));
		KeyForwardSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultForward));
		OnSelectorKeyForwardChange(FKey(*PestilenceGameUserSettings->KeyDefaultForward));
		KeyLeftSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultLeft));
		OnSelectorKeyLeftChange(FKey(*PestilenceGameUserSettings->KeyDefaultLeft));

		PadCrouchSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultCrouch));
		OnSelectorPadCrouchChange(FKey(*PestilenceGameUserSettings->PadDefaultCrouch));
		KeyCrouchSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultCrouch));
		OnSelectorKeyCrouchChange(FKey(*PestilenceGameUserSettings->KeyDefaultCrouch));

		PadRollSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultRoll));
		OnSelectorPadRollChange(FKey(*PestilenceGameUserSettings->PadDefaultRoll));
		KeyRollSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultRoll));
		OnSelectorKeyRollChange(FKey(*PestilenceGameUserSettings->KeyDefaultRoll));

		KeyWalkSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultWalk));
		OnSelectorKeyWalkChange(FKey(*PestilenceGameUserSettings->KeyDefaultWalk));

		PadInteractSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultInteract));
		OnSelectorPadInteractChange(FKey(*PestilenceGameUserSettings->PadDefaultInteract));
		KeyInteractSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultInteract));
		OnSelectorKeyInteractChange(FKey(*PestilenceGameUserSettings->KeyDefaultInteract));

		PadLeanRightSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultLeanRight));
		OnSelectorPadLeanRightChange(FKey(*PestilenceGameUserSettings->PadDefaultLeanRight));
		KeyLeanRightSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultLeanRight));
		OnSelectorKeyLeanRightChange(FKey(*PestilenceGameUserSettings->KeyDefaultLeanRight));

		PadLeanLeftSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultLeanLeft));
		OnSelectorPadLeanLeftChange(FKey(*PestilenceGameUserSettings->PadDefaultLeanLeft));
		KeyLeanLeftSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultLeanLeft));
		OnSelectorKeyLeanLeftChange(FKey(*PestilenceGameUserSettings->KeyDefaultLeanLeft));

		KeySaveSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultSave));
		OnSelectorKeySaveChange(FKey(*PestilenceGameUserSettings->KeyDefaultSave));

		KeyLoadSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultLoad));
		OnSelectorKeyLoadChange(FKey(*PestilenceGameUserSettings->KeyDefaultLoad));

		PadAbilityCastSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultAbilityCast));
		OnSelectorPadAbilityCastChange(FKey(*PestilenceGameUserSettings->PadDefaultAbilityCast));
		KeyAbilityCastSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultAbilityCast));
		OnSelectorKeyAbilityCastChange(FKey(*PestilenceGameUserSettings->KeyDefaultAbilityCast));

		PadAbilityPrepareSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultAbilityPrepare));
		OnSelectorPadAbilityPrepareChange(FKey(*PestilenceGameUserSettings->PadDefaultAbilityPrepare));
		KeyAbilityPrepareSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultAbilityPrepare));
		OnSelectorKeyAbilityPrepareChange(FKey(*PestilenceGameUserSettings->KeyDefaultAbilityPrepare));

		PadAbility1Selector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultAbility1));
		OnSelectorPadAbility1Change(FKey(*PestilenceGameUserSettings->PadDefaultAbility1));

		KeyAbility1Selector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultAbility1));
		OnSelectorKeyAbility1Change(FKey(*PestilenceGameUserSettings->KeyDefaultAbility1));

		PadAbility2Selector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultAbility2));
		OnSelectorPadAbility2Change(FKey(*PestilenceGameUserSettings->PadDefaultAbility2));

		KeyAbility2Selector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultAbility2));
		OnSelectorKeyAbility2Change(FKey(*PestilenceGameUserSettings->KeyDefaultAbility2));

		PadAbility3Selector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultAbility3));
		OnSelectorPadAbility3Change(FKey(*PestilenceGameUserSettings->PadDefaultAbility3));

		KeyAbility3Selector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultAbility3));
		OnSelectorKeyAbility3Change(FKey(*PestilenceGameUserSettings->KeyDefaultAbility3));

		PadAbility4Selector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultAbility4));
		OnSelectorPadAbility4Change(FKey(*PestilenceGameUserSettings->PadDefaultAbility4));

		KeyAbility4Selector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultAbility4));
		OnSelectorKeyAbility4Change(FKey(*PestilenceGameUserSettings->KeyDefaultAbility4));

		KeyAbilityNextSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultAbilitySelectNext));
		OnSelectorKeyAbilitySelectNextChange(FKey(*PestilenceGameUserSettings->KeyDefaultAbilitySelectNext));

		KeyAbilityPrevSelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultAbilitySelectPrev));
		OnSelectorKeyAbilitySelectPrevChange(FKey(*PestilenceGameUserSettings->KeyDefaultAbilitySelectPrev));

		PadDiarySelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->PadDefaultDiary));
		OnSelectorPadDiaryChange(FKey(*PestilenceGameUserSettings->PadDefaultDiary));
		KeyDiarySelector->SetSelectedKey(FKey(*PestilenceGameUserSettings->KeyDefaultDiary));
		OnSelectorKeyDiaryChange(FKey(*PestilenceGameUserSettings->KeyDefaultDiary));
	}
}

void UPestilenceOptionsMenu::SetDefaultInputs()
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings && IsValid(InputMappingContext) && IsValid(DefaultInputMappingContext) && IsValid(PossessionInputMappingContext) && IsValid(LockpickInputMappingContext))
	{
		UnMapContext(InputMappingContext);

		TArray<FEnhancedActionKeyMapping> Maps = DefaultInputMappingContext->GetMappings();

		for (int i = 0; i < Maps.Num(); ++i)
		{
			if (Maps[i].Action == MoveAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					continue;
				}

				else
				{
					if (Maps[i].Modifiers.IsEmpty())
					{
						KeyRightSelector->SetSelectedKey(Maps[i].Key);
						OnSelectorKeyRightChange(Maps[i].Key);
					}

					else if (Maps[i].Modifiers.Num() == 2)
					{
						KeyBackSelector->SetSelectedKey(Maps[i].Key);
						OnSelectorKeyBackChange(Maps[i].Key);
					}

					else
					{
						for (int j = 0; j < Maps[i].Modifiers.Num(); ++j)
						{
							UInputModifierSwizzleAxis* InputModifierSwizzleAxis = Cast<UInputModifierSwizzleAxis>(Maps[i].Modifiers[j]);
							if (InputModifierSwizzleAxis)
							{
								KeyForwardSelector->SetSelectedKey(Maps[i].Key);
								OnSelectorKeyForwardChange(Maps[i].Key);
							}

							UInputModifierNegate* InputModifierNegate = Cast<UInputModifierNegate>(Maps[i].Modifiers[j]);
							if (InputModifierNegate)
							{
								KeyLeftSelector->SetSelectedKey(Maps[i].Key);
								OnSelectorKeyLeftChange(Maps[i].Key);
							}
						}
					}
				}
			}

			else if (Maps[i].Action == CrouchAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadCrouchSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadCrouchChange(Maps[i].Key);
				}

				else
				{
					KeyCrouchSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyCrouchChange( Maps[i].Key);
				}
			}

			else if (Maps[i].Action == RollAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadRollSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadRollChange(Maps[i].Key);
				}

				else
				{
					KeyRollSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyRollChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == WalkAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					continue;
				}

				else
				{
					KeyWalkSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyWalkChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == InteractAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadInteractSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadInteractChange(Maps[i].Key);
				}

				else
				{
					KeyInteractSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyInteractChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == LeanRightAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadLeanRightSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadLeanRightChange(Maps[i].Key);
				}

				else
				{
					KeyLeanRightSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyLeanRightChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == LeanLeftAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadLeanLeftSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadLeanLeftChange(Maps[i].Key);
				}

				else
				{
					KeyLeanLeftSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyLeanLeftChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == SaveAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					continue;
				}

				else
				{
					KeySaveSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeySaveChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == LoadAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					continue;
				}

				else
				{
					KeyLoadSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyLoadChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == AbilityCastAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadAbilityCastSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadAbilityCastChange(Maps[i].Key);
				}

				else
				{
					KeyAbilityCastSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyAbilityCastChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == AbilityPrepareAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadAbilityPrepareSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadAbilityPrepareChange(Maps[i].Key);
				}

				else
				{
					KeyAbilityPrepareSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyAbilityPrepareChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == AbilitySelect1Action)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadAbility1Selector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadAbility1Change(Maps[i].Key);
				}

				else
				{
					KeyAbility1Selector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyAbility1Change(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == AbilitySelect2Action)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadAbility2Selector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadAbility2Change(Maps[i].Key);
				}

				else
				{
					KeyAbility2Selector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyAbility2Change(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == AbilitySelect3Action)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadAbility3Selector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadAbility3Change(Maps[i].Key);
				}

				else
				{
					KeyAbility3Selector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyAbility3Change(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == AbilitySelect4Action)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadAbility4Selector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadAbility4Change(Maps[i].Key);
				}

				else
				{
					KeyAbility4Selector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyAbility4Change(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == AbilitySelectNextAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					continue;
				}

				else
				{
					KeyAbilityNextSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyAbilitySelectNextChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == AbilitySelectPrevAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					continue;
				}

				else
				{
					KeyAbilityPrevSelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyAbilitySelectPrevChange(Maps[i].Key);
				}
			}

			else if (Maps[i].Action == DiaryAction)
			{
				if (Maps[i].Key.IsGamepadKey())
				{
					PadDiarySelector->SetSelectedKey(Maps[i].Key);
					OnSelectorPadDiaryChange(Maps[i].Key);
				}

				else
				{
					KeyDiarySelector->SetSelectedKey(Maps[i].Key);
					OnSelectorKeyDiaryChange(Maps[i].Key);
				}
			}
		}
	}
}

void UPestilenceOptionsMenu::UnMapContext(UInputMappingContext* _InputMappingContext)
{
	_InputMappingContext->UnmapAll(); 
	
	TArray<FEnhancedActionKeyMapping> Maps = DefaultInputMappingContext->GetMappings();

	for (int i = 0; i < Maps.Num(); ++i)
	{
		FEnhancedActionKeyMapping& input = _InputMappingContext->MapKey(Maps[i].Action, Maps[i].Key);
		for (int j = 0; j < Maps[i].Modifiers.Num(); ++j) 
		{
			input.Modifiers.Add(Maps[i].Modifiers[j]);
		}

		for (int j = 0; j < Maps[i].Triggers.Num(); ++j)
		{
			input.Triggers.Add(Maps[i].Triggers[j]);
		}
	}

	_InputMappingContext->UnmapAllKeysFromAction(MoveAction);

	FEnhancedActionKeyMapping& InputMove = _InputMappingContext->MapKey(MoveAction, EKeys::Gamepad_Left2D);
	UInputModifierDeadZone* InputModifierDeadZone = NewObject<UInputModifierDeadZone>();
	InputMove.Modifiers.Add(InputModifierDeadZone);

	LockpickInputMappingContext->UnmapAllKeysFromAction(LockpickMoveAction);

	FEnhancedActionKeyMapping& LockpickInputMove = LockpickInputMappingContext->MapKey(LockpickMoveAction, EKeys::Gamepad_Left2D);
	UInputModifierDeadZone* InputModifierDeadZoneLockpick = NewObject<UInputModifierDeadZone>();
	UInputModifierScalar* InputModifierScalar = NewObject<UInputModifierScalar>(); 
	InputModifierScalar->Scalar = FVector(50, 50, 1);
	LockpickInputMove.Modifiers.Add(InputModifierDeadZoneLockpick);
	LockpickInputMove.Modifiers.Add(InputModifierScalar);

	_InputMappingContext->UnmapAllKeysFromAction(CrouchAction);
	_InputMappingContext->UnmapAllKeysFromAction(WalkAction);
	_InputMappingContext->UnmapAllKeysFromAction(RollAction);
	_InputMappingContext->UnmapAllKeysFromAction(InteractAction);
	_InputMappingContext->UnmapAllKeysFromAction(InteractUIAction);
	_InputMappingContext->UnmapAllKeysFromAction(LeanRightAction);
	_InputMappingContext->UnmapAllKeysFromAction(LeanLeftAction);
	_InputMappingContext->UnmapAllKeysFromAction(SaveAction);
	_InputMappingContext->UnmapAllKeysFromAction(LoadAction);

#if WITH_EDITOR
	FEnhancedActionKeyMapping& InputSave = InputMappingContext->MapKey(SaveAction, EKeys::K);
	FEnhancedActionKeyMapping& InputLoad = InputMappingContext->MapKey(LoadAction, EKeys::L);
#endif

	_InputMappingContext->UnmapAllKeysFromAction(AbilityCastAction);
	_InputMappingContext->UnmapAllKeysFromAction(AbilityPrepareAction);
	PossessionInputMappingContext->UnmapAllKeysFromAction(AbilityCastAction);
	PossessionInputMappingContext->UnmapAllKeysFromAction(AbilityPrepareAction);
	LockpickInputMappingContext->UnmapAllKeysFromAction(AbilityPrepareAction);
	_InputMappingContext->UnmapAllKeysFromAction(AbilitySelect1Action);
	_InputMappingContext->UnmapAllKeysFromAction(AbilitySelect2Action);
	_InputMappingContext->UnmapAllKeysFromAction(AbilitySelect3Action);
	_InputMappingContext->UnmapAllKeysFromAction(AbilitySelect4Action);
	_InputMappingContext->UnmapAllKeysFromAction(AbilitySelectNextAction);
	_InputMappingContext->UnmapAllKeysFromAction(AbilitySelectPrevAction);
	_InputMappingContext->UnmapAllKeysFromAction(DiaryAction);
	LockpickInputMappingContext->UnmapAllKeysFromAction(DiaryAction);
}

void UPestilenceOptionsMenu::GameplayConfig(bool InvertKeyX, bool InvertKeyY, bool InvertPadX, bool InvertPadY, float SensitivityKeyX, float SensitivityKeyY, 
	float SensitivityPadX, float SensitivityPadY, float AccelerationKeyX, float AccelerationKeyY, float AccelerationPadX, float AccelerationPadY, 
	bool bVibration, bool bHeadBobbing)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->bPestilenceInvertKeyX = InvertKeyX;
		PestilenceGameUserSettings->bPestilenceInvertKeyY = InvertKeyY;
		PestilenceGameUserSettings->bPestilenceInvertPadX = InvertPadX;
		PestilenceGameUserSettings->bPestilenceInvertPadY = InvertPadY;

		KeyXAxisCheckBox->SetIsChecked(InvertKeyX);
		KeyYAxisCheckBox->SetIsChecked(InvertKeyY);
		PadXAxisCheckBox->SetIsChecked(InvertPadX);
		PadYAxisCheckBox->SetIsChecked(InvertPadY);

		PestilenceGameUserSettings->PestilenceSensitivityKeyX = SensitivityKeyX;
		PestilenceGameUserSettings->PestilenceSensitivityKeyY = SensitivityKeyY;
		PestilenceGameUserSettings->PestilenceSensitivityPadX = SensitivityPadX;
		PestilenceGameUserSettings->PestilenceSensitivityPadY = SensitivityPadY;

		SensitivityKeyXSlider->SetValue(SensitivityKeyX);
		SensitivityKeyYSlider->SetValue(SensitivityKeyY);
		SensitivityPadXSlider->SetValue(SensitivityPadX);
		SensitivityPadYSlider->SetValue(SensitivityPadY);

    PestilenceGameUserSettings->PestilenceCameraAccelKeyX = AccelerationKeyX;
    PestilenceGameUserSettings->PestilenceCameraAccelKeyY = AccelerationKeyY;
    PestilenceGameUserSettings->PestilenceCameraAccelPadX = AccelerationPadX;
    PestilenceGameUserSettings->PestilenceCameraAccelPadY = AccelerationPadY;

    AccelerationKeyXSlider->SetValue(AccelerationKeyX);
		AccelerationKeyYSlider->SetValue(AccelerationKeyY);
		AccelerationPadXSlider->SetValue(AccelerationPadX);
		AccelerationPadYSlider->SetValue(AccelerationPadY);

		PestilenceGameUserSettings->bHeadBobbing = bHeadBobbing;
		HeadBobbingCheckBox->SetIsChecked(bHeadBobbing);
 
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PadVibrationCheckBox->SetIsChecked(bVibration);
			PestilenceGameUserSettings->bIsVibrationPad = bVibration; 
			PlayerController->bForceFeedbackEnabled = bVibration;
		}
	}
}

void UPestilenceOptionsMenu::OnComboQualityComboBoxSelectionChanged(Scalability::FQualityLevels Quality)
{
	OnComboAntialiasingComboBoxSelectionChanged(AAGraphicsQuality);
	OnComboGlobalIlluminationComboBoxSelectionChanged(GlobalIlluminationQuality);
	OnComboShadowComboBoxSelectionChanged(ShadowGraphicsQuality);
	OnComboTextureComboBoxSelectionChanged(TextureGraphicsQuality);
	OnComboVFXSelectionChanged(VFXGraphicsQuality);
	OnComboLevelDetailSelectionChanged(LevelDetailGraphicsQuality);
	OnComboPostProccessSelectionChanged(PPGraphicsQuality);

	if (AAGraphicsQuality == GlobalIlluminationQuality && GlobalIlluminationQuality == ShadowGraphicsQuality && ShadowGraphicsQuality == TextureGraphicsQuality && 
			TextureGraphicsQuality == VFXGraphicsQuality && VFXGraphicsQuality == LevelDetailGraphicsQuality && LevelDetailGraphicsQuality == PPGraphicsQuality)
	{
		QualityComboBox->SetSelectedIndex(AAGraphicsQuality);
		GeneralQualityComboBox->SetSelectedIndex(AAGraphicsQuality);
	}

	else
	{
		QualityComboBox->SetSelectedOption(TEXT("Custom"));
		GeneralQualityComboBox->SetSelectedOption(TEXT("Custom"));
	}
}

void UPestilenceOptionsMenu::OnComboAntialiasingComboBoxSelectionChanged(int32 SelectedItem)
{
	FString Command = FString::Printf(TEXT("sg.AntiAliasingQuality %d"), SelectedItem);

	AntialiasingComboBox->SetSelectedIndex(SelectedItem);

	AAGraphicsQuality = SelectedItem;
}

void UPestilenceOptionsMenu::OnComboGlobalIlluminationComboBoxSelectionChanged(int32 SelectedItem)
{
  FString Command = FString::Printf(TEXT("sg.GlobalIlluminationQuality %d"), SelectedItem);

  GlobalIlluminationComboBox->SetSelectedIndex(SelectedItem);

  GlobalIlluminationQuality = SelectedItem;
}

void UPestilenceOptionsMenu::OnComboShadowComboBoxSelectionChanged(int32 SelectedItem)
{
  FString Command = FString::Printf(TEXT("sg.ShadowQuality %d"), SelectedItem);

  ShadowComboBox->SetSelectedIndex(SelectedItem);

  ShadowGraphicsQuality = SelectedItem;
}

void UPestilenceOptionsMenu::OnComboTextureComboBoxSelectionChanged(int32 SelectedItem)
{
	FString Command = FString::Printf(TEXT("sg.TextureQuality %d"), SelectedItem);

	TextureComboBox->SetSelectedIndex(SelectedItem);

	TextureGraphicsQuality = SelectedItem;
}

void UPestilenceOptionsMenu::OnComboVFXSelectionChanged(int32 SelectedItem)
{
	FString Command = FString::Printf(TEXT("sg.EffectsQuality %d"), SelectedItem);

	VFXComboBox->SetSelectedIndex(SelectedItem);

	VFXGraphicsQuality = SelectedItem;
}

void UPestilenceOptionsMenu::OnComboLevelDetailSelectionChanged(int32 SelectedItem)
{
	FString Command = FString::Printf(TEXT("sg.ViewDistanceQuality %d"), SelectedItem);

	LevelDetailComboBox->SetSelectedIndex(SelectedItem);

	LevelDetailGraphicsQuality = SelectedItem;
}

void UPestilenceOptionsMenu::OnComboPostProccessSelectionChanged(int32 SelectedItem)
{
	FString Command = FString::Printf(TEXT("sg.PostProcessQuality %d"), SelectedItem);

	PostProccessComboBox->SetSelectedIndex(SelectedItem);

	PPGraphicsQuality = SelectedItem;
}

void UPestilenceOptionsMenu::Options()
{
	OptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GameplayOptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GraphicsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SoundOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ControlsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	GamepadOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyBoardOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyControlsPanel->SetVisibility(ESlateVisibility::Hidden);
	PadControlsPanel->SetVisibility(ESlateVisibility::Hidden);

	SetOptionsFocusInit();
}

void UPestilenceOptionsMenu::OptionsKey()
{
	OptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GameplayOptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GraphicsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SoundOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ControlsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	GamepadOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyBoardOptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	KeyControlsPanel->SetVisibility(ESlateVisibility::Hidden);
	PadControlsPanel->SetVisibility(ESlateVisibility::Hidden);

	SetOptionsFocusInit();
}

void UPestilenceOptionsMenu::OptionsPad()
{
	OptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GameplayOptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GraphicsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SoundOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ControlsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	GamepadOptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	KeyBoardOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyControlsPanel->SetVisibility(ESlateVisibility::Hidden);
	PadControlsPanel->SetVisibility(ESlateVisibility::Hidden);

	SetOptionsFocusInit();
}

void UPestilenceOptionsMenu::OptionsKeyControl()
{
	OptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GameplayOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	GraphicsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SoundOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ControlsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	GamepadOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyBoardOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyControlsPanel->SetVisibility(ESlateVisibility::Visible);
	PadControlsPanel->SetVisibility(ESlateVisibility::Hidden);

	KeyForwardSelector->SetFocus();
}

void UPestilenceOptionsMenu::OptionsPadControl()
{
	OptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GameplayOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	GraphicsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SoundOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ControlsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	GamepadOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyBoardOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyControlsPanel->SetVisibility(ESlateVisibility::Hidden);
	PadControlsPanel->SetVisibility(ESlateVisibility::Visible);

	PadCrouchSelector->SetFocus();
}

void UPestilenceOptionsMenu::OptionsVideo()
{
	OptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GameplayOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	GraphicsOptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	SoundOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ControlsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	GamepadOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyBoardOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyControlsPanel->SetVisibility(ESlateVisibility::Hidden);
	PadControlsPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UPestilenceOptionsMenu::OptionsSound()
{
	OptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GameplayOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	GraphicsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SoundOptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	ControlsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	GamepadOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyBoardOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyControlsPanel->SetVisibility(ESlateVisibility::Hidden);
	PadControlsPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UPestilenceOptionsMenu::CloseOptions()
{
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		float auxMarterVolume;
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceMasterVolume"), auxMarterVolume, GGameIni);

		float auxVFXVolume;
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceEffectsVolume"), auxVFXVolume, GGameIni);

		float auxVoicesVolume;
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceVoicesVolume"), auxVoicesVolume, GGameIni);

		float auxMusicVolume;
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceMusicVolume"), auxMusicVolume, GGameIni);

		float auxFOV;
		GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceFOV"), auxFOV, GGameIni);

    int32 AuxResolutionX, AuxResolutionY;
    GConfig->GetInt(TEXT("PestilenceGroups"), TEXT("ResolutionX"), AuxResolutionX, GGameIni);
    GConfig->GetInt(TEXT("PestilenceGroups"), TEXT("ResolutionY"), AuxResolutionY, GGameIni);

    int32 auxFidelityFXQuality;
    GConfig->GetInt(TEXT("PestilenceGroups"), TEXT("FidelityFXQuality"), auxFidelityFXQuality, GGameIni);
    float auxFidelityFXSharpness;
    GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("FidelityFXSharpness"), auxFidelityFXSharpness, GGameIni);

		float AuxResolutionNormalize, AuxResolution, AuxResolutionMin, AuxResolutionMax;

		GameUserSettings->GetResolutionScaleInformationEx(AuxResolutionNormalize, AuxResolution, AuxResolutionMin, AuxResolutionMax);

		UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
		if (PestilenceGameUserSettings) 
		{
			bool bAuxInvertKeyX, bAuxInvertKeyY, bAuxInvertPadX, bAuxInvertPadY, bIsVibrationPad, bHeadBobbing;
			float AuxSensitivityKeyX, AuxSensitivityKeyY, AuxSensitivityPadX, AuxSensitivityPadY,
				AuxAccelerationKeyX, AuxAccelerationKeyY, AuxAccelerationPadX, AuxAccelerationPadY;

			GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertKeyX"), bAuxInvertKeyX, GGameIni);
			GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertKeyY"), bAuxInvertKeyY, GGameIni);
			GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertPadX"), bAuxInvertPadX, GGameIni);
      GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertPadY"), bAuxInvertPadY, GGameIni);
      GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityKeyX"), AuxSensitivityKeyX, GGameIni);
      GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityKeyY"), AuxSensitivityKeyY, GGameIni);
      GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityPadX"), AuxSensitivityPadX, GGameIni);
      GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityPadY"), AuxSensitivityPadY, GGameIni);
      GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelKeyX"), AuxAccelerationKeyX, GGameIni);
      GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelKeyY"), AuxAccelerationKeyY, GGameIni);
      GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelPadX"), AuxAccelerationPadX, GGameIni);
      GConfig->GetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelPadY"), AuxAccelerationPadY, GGameIni);
			GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bIsVibrationPad"), bIsVibrationPad, GGameIni);
			GConfig->GetBool(TEXT("PestilenceGroups"), TEXT("bHeadBobbing"), bHeadBobbing, GGameIni);

			if (GeneralVolumeSlider->GetValue() == auxMarterVolume && VFXVolumeSlider->GetValue() == auxVFXVolume && VoicesVolumeSlider->GetValue() == auxVoicesVolume &&
				MusicVolumeSlider->GetValue() == auxMusicVolume && AAGraphicsQuality == Scalability::GetQualityLevels().AntiAliasingQuality &&
				PPGraphicsQuality == Scalability::GetQualityLevels().PostProcessQuality && VFXGraphicsQuality == Scalability::GetQualityLevels().EffectsQuality &&
				TextureGraphicsQuality == Scalability::GetQualityLevels().TextureQuality && GlobalIlluminationQuality == Scalability::GetQualityLevels().GlobalIlluminationQuality &&
				ShadowGraphicsQuality == Scalability::GetQualityLevels().ShadowQuality &&
				LevelDetailGraphicsQuality == Scalability::GetQualityLevels().ViewDistanceQuality && FPS == GameUserSettings->GetFrameRateLimit() &&
				AuxResolutionX == ResolutionX && AuxResolutionY == ResolutionY && 
				auxFidelityFXQuality == FidelityFXQuality && auxFidelityFXSharpness == FidelityFXSharpnessSlider->GetValue() && 
				ScaleResolutionSlider->GetValue() == AuxResolution && DistanceResolutionSlider->GetValue() == auxFOV && SubtitlesCheckBox->IsChecked() == bSubtitle &&
				KeyXAxisCheckBox->IsChecked() == bAuxInvertKeyX && KeyYAxisCheckBox->IsChecked() == bAuxInvertKeyY && PadXAxisCheckBox->IsChecked() == bAuxInvertPadX && PadYAxisCheckBox->IsChecked() == bAuxInvertPadY &&
        SensitivityKeyXSlider->GetValue() == AuxSensitivityKeyX && SensitivityKeyYSlider->GetValue() == AuxSensitivityKeyY &&
        SensitivityPadXSlider->GetValue() == AuxSensitivityPadX && SensitivityPadYSlider->GetValue() == AuxSensitivityPadY &&
        AccelerationKeyXSlider->GetValue() == AuxAccelerationKeyX && AccelerationKeyYSlider->GetValue() == AuxAccelerationKeyY &&
				AccelerationPadXSlider->GetValue() == AuxAccelerationPadX && AccelerationPadYSlider->GetValue() == AuxAccelerationPadY &&
				PadVibrationCheckBox->IsChecked() == bIsVibrationPad && HeadBobbingCheckBox->IsChecked() == bHeadBobbing &&
				PestilenceGameUserSettings->KeyDefaultForward == PestilenceGameUserSettings->KeyForwardKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultLeft == PestilenceGameUserSettings->KeyLeftKey.ToString() && 
				PestilenceGameUserSettings->KeyDefaultBack == PestilenceGameUserSettings->KeyBackKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultRoll == PestilenceGameUserSettings->KeyRollKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultRight == PestilenceGameUserSettings->KeyRightKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultCrouch == PestilenceGameUserSettings->KeyCrouchKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultWalk == PestilenceGameUserSettings->KeyWalkKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultInteract == PestilenceGameUserSettings->KeyInteractKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultLeanRight == PestilenceGameUserSettings->KeyLeanRightKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultLeanLeft == PestilenceGameUserSettings->KeyLeanLeftKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultSave == PestilenceGameUserSettings->KeySaveKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultLoad == PestilenceGameUserSettings->KeyLoadKey.ToString() && 
				PestilenceGameUserSettings->KeyDefaultAbilityCast == PestilenceGameUserSettings->KeyAbilityCastKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultAbilityPrepare == PestilenceGameUserSettings->KeyAbilityPrepareKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultAbility1 == PestilenceGameUserSettings->KeyAbility1Key.ToString() &&
				PestilenceGameUserSettings->KeyDefaultAbility2 == PestilenceGameUserSettings->KeyAbility2Key.ToString() &&
				PestilenceGameUserSettings->KeyDefaultAbility3 == PestilenceGameUserSettings->KeyAbility3Key.ToString() &&
				PestilenceGameUserSettings->KeyDefaultAbility4 == PestilenceGameUserSettings->KeyAbility4Key.ToString() &&
				PestilenceGameUserSettings->KeyDefaultAbilitySelectNext == PestilenceGameUserSettings->KeyAbilitySelectNextKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultAbilitySelectPrev == PestilenceGameUserSettings->KeyAbilitySelectPrevKey.ToString() &&
				PestilenceGameUserSettings->KeyDefaultDiary == PestilenceGameUserSettings->KeyDiaryKey.ToString() &&
				PestilenceGameUserSettings->PadDefaultRoll == PestilenceGameUserSettings->PadRollKey.ToString() &&
				PestilenceGameUserSettings->PadDefaultCrouch == PestilenceGameUserSettings->PadCrouchKey.ToString() &&
				PestilenceGameUserSettings->PadDefaultInteract == PestilenceGameUserSettings->PadInteractKey.ToString() &&
				PestilenceGameUserSettings->PadDefaultLeanRight == PestilenceGameUserSettings->PadLeanRightKey.ToString() &&
				PestilenceGameUserSettings->PadDefaultLeanLeft == PestilenceGameUserSettings->PadLeanLeftKey.ToString() &&
				PestilenceGameUserSettings->PadDefaultAbilityCast == PestilenceGameUserSettings->PadAbilityCastKey.ToString() &&
				PestilenceGameUserSettings->PadDefaultAbilityPrepare == PestilenceGameUserSettings->PadAbilityPrepareKey.ToString() &&
				PestilenceGameUserSettings->PadDefaultAbility1 == PestilenceGameUserSettings->PadAbility1Key.ToString() &&
				PestilenceGameUserSettings->PadDefaultAbility2 == PestilenceGameUserSettings->PadAbility2Key.ToString() &&
				PestilenceGameUserSettings->PadDefaultAbility3 == PestilenceGameUserSettings->PadAbility3Key.ToString() &&
				PestilenceGameUserSettings->PadDefaultAbility4 == PestilenceGameUserSettings->PadAbility4Key.ToString() &&
				PestilenceGameUserSettings->PadDefaultDiary == PestilenceGameUserSettings->PadDiaryKey.ToString())
			{
				Exit();
			}

			else
			{
				OptionMenu = EnumOption::ConfirmationExit;
				ConfimationText->SetText(FText::FromString(ConfirmationTextExit));
				ConfirmationPanel->SetVisibility(ESlateVisibility::Visible);

				TArray<UWidget*> Children;

				ConfirmationNoButton->WidgetTree->GetAllWidgets(Children);
				for (auto Child : Children)
				{
					if (UButton* Button = Cast<UButton>(Child))
					{
						Button->SetFocus();
					}
				}
				Children.Empty();
			}
		}
	}
}

void UPestilenceOptionsMenu::Exit()
{
  Options();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && PlayerController->GetHUD())
	{
		APestilenceHUD* PestilenceHUD = Cast<APestilenceHUD>(PlayerController->GetHUD());
		if (PestilenceHUD)
		{
			switch (OptionGoMenu)
			{
			case EnumOptionGoMenu::MainMenu:
				//PestilenceHUD->ShowWidget(PestilenceHUD->MainMenuWidgetInstance);
				//PestilenceHUD->MainMenuWidgetInstance->StartNewGameButton->SetFocus();
				OnExitOptions.Broadcast();
				break;

			case EnumOptionGoMenu::Pause:
				PestilenceHUD->ShowWidget(PestilenceHUD->PauseMenuWidgetInstance);
				PestilenceHUD->PauseMenuWidgetInstance->ContinueGameButton->SetFocus();

				break;
			case EnumOptionGoMenu::GameOver:
				PestilenceHUD->ShowWidget(PestilenceHUD->GameOverWidgetInstance);

				break;
			default:
				OnExitOptions.Broadcast();
				break;
			}
			AppliedChanges();
			if(OptionGoMenu != EnumOptionGoMenu::MainMenu)
				RemoveFromParent();
		}
	}
}

void UPestilenceOptionsMenu::OptionsControls()
{
	OptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	GameplayOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	GraphicsOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SoundOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	ControlsOptionsMenuPanel->SetVisibility(ESlateVisibility::Visible);
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
	GamepadOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyBoardOptionsMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	KeyControlsPanel->SetVisibility(ESlateVisibility::Hidden);
	PadControlsPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UPestilenceOptionsMenu::OptionsConfirmation()
{
	OptionMenu = EnumOption::Confirmation;
	ConfimationText->SetText(FText::FromString(ConfirmationSaveText));
	ConfirmationPanel->SetVisibility(ESlateVisibility::Visible);

	TArray<UWidget*> Children;

	ConfirmationNoButton->WidgetTree->GetAllWidgets(Children);
	for (auto Child : Children)
	{
		if (UButton* Button = Cast<UButton>(Child))
		{
			Button->SetFocus();
		}
	}
	Children.Empty();
}

void UPestilenceOptionsMenu::OptionsDefault()
{
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();

	if (GameUserSettings)
	{
		GameUserSettings->RunHardwareBenchmark();
		GameUserSettings->ApplyHardwareBenchmarkResults();

		FTimerDelegate TimerDelegate;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				OnComboQualityComboBoxSelectionChanged(Scalability::GetQualityLevels());
			}, 0.2f, false);
		GameUserSettings->SetScreenResolution(GameUserSettings->GetDesktopResolution());
		GameUserSettings->ApplyResolutionSettings(true);
		ResolutionComboBox->SetSelectedOption(FString::Printf(TEXT("%dx%d"), GameUserSettings->GetDesktopResolution().X, GameUserSettings->GetDesktopResolution().Y));
	}

	SubtitlesCheckBox->SetIsChecked(true);

	ScaleResolutionSlider->SetValue(100);

	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (PlayerCameraManager)
	{
		DistanceResolutionSlider->SetValue(90);
		PlayerCameraManager->SetFOV(90);
	}

	if (IsValid(Master))
	{
		GeneralVolumeSlider->SetValue(1.f);
		Master->Properties.Volume = 1.f;
	}
	if (IsValid(VFX))
	{
		VFXVolumeSlider->SetValue(1.f);
		VFX->Properties.Volume = 1.f;
	}
	if (IsValid(Master))
	{
		VoicesVolumeSlider->SetValue(1.f);
		Voices->Properties.Volume = 1.f;
	}
	if (IsValid(Music))
	{
		MusicVolumeSlider->SetValue(1.f);
		Music->Properties.Volume = 1.f;
	}

	SetDefaultInputs();
	GameplayConfig(); 
}

void UPestilenceOptionsMenu::OptionsConfirmationYes()
{
	ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);

	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceFOV"), DistanceResolutionSlider->GetValue(), GGameIni);

		if (IsValid(Master))
		{
			GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceMasterVolume"), GeneralVolumeSlider->GetValue(), GGameIni);
		}
		if (IsValid(VFX))
		{
			GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceEffectsVolume"), VFXVolumeSlider->GetValue(), GGameIni);
		}
		if (IsValid(Master))
		{
			GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceVoicesVolume"), VoicesVolumeSlider->GetValue(), GGameIni);
		}
		if (IsValid(Music))
		{
			GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceMusicVolume"), MusicVolumeSlider->GetValue(), GGameIni);
		}

		GConfig->SetBool(TEXT("PestilenceGroups"), TEXT("bHaveSave"), true, GGameIni);

		GConfig->SetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertKeyX"), KeyXAxisCheckBox->IsChecked(), GGameIni);
		GConfig->SetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertKeyY"), KeyYAxisCheckBox->IsChecked(), GGameIni);
		GConfig->SetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertPadX"), PadXAxisCheckBox->IsChecked(), GGameIni);
    GConfig->SetBool(TEXT("PestilenceGroups"), TEXT("bPestilenceInvertPadY"), PadYAxisCheckBox->IsChecked(), GGameIni);
    GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityKeyX"), SensitivityKeyXSlider->GetValue(), GGameIni);
    GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityKeyY"), SensitivityKeyYSlider->GetValue(), GGameIni);
    GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityPadX"), SensitivityPadXSlider->GetValue(), GGameIni);
    GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceSensitivityPadY"), SensitivityPadYSlider->GetValue(), GGameIni);
    GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelKeyX"), AccelerationKeyXSlider->GetValue(), GGameIni);
    GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelKeyY"), AccelerationKeyYSlider->GetValue(), GGameIni);
    GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelPadX"), AccelerationPadXSlider->GetValue(), GGameIni);
    GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("PestilenceCameraAccelPadY"), AccelerationPadYSlider->GetValue(), GGameIni);
		GConfig->SetBool(TEXT("PestilenceGroups"), TEXT("bIsVibrationPad"), PadVibrationCheckBox->IsChecked(), GGameIni);
		GConfig->SetBool(TEXT("PestilenceGroups"), TEXT("bHeadBobbing"), HeadBobbingCheckBox->IsChecked(), GGameIni);

		GameUserSettings->SetFrameRateLimit(FPS);

    if (FidelityFXQuality == 0)
      GameUserSettings->SetResolutionScaleValueEx(ScaleResolutionSlider->GetValue());
		else
      GameUserSettings->SetResolutionScaleValueEx(100.f);

		AAGraphicsQuality = Scalability::GetQualityLevels().AntiAliasingQuality;
		PPGraphicsQuality = Scalability::GetQualityLevels().PostProcessQuality;
		VFXGraphicsQuality = Scalability::GetQualityLevels().EffectsQuality;
		TextureGraphicsQuality = Scalability::GetQualityLevels().TextureQuality;
		GlobalIlluminationQuality = Scalability::GetQualityLevels().GlobalIlluminationQuality;
		ShadowGraphicsQuality = Scalability::GetQualityLevels().ShadowQuality;
		LevelDetailGraphicsQuality = Scalability::GetQualityLevels().ViewDistanceQuality;

    GameUserSettings->SaveSettings();

    GConfig->SetInt(TEXT("PestilenceGroups"), TEXT("ResolutionX"), ResolutionX, GGameIni);
    GConfig->SetInt(TEXT("PestilenceGroups"), TEXT("ResolutionY"), ResolutionY, GGameIni);
    GConfig->SetInt(TEXT("PestilenceGroups"), TEXT("FidelityFXQuality"), FidelityFXQuality, GGameIni);
    GConfig->SetFloat(TEXT("PestilenceGroups"), TEXT("FidelityFXSharpness"), FidelityFXSharpnessSlider->GetValue(), GGameIni);

		UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
		if (PestilenceGameUserSettings) 
		{
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultForwardKey"), *PestilenceGameUserSettings->KeyForwardKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLeftKey"), *PestilenceGameUserSettings->KeyLeftKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultBackKey"), *PestilenceGameUserSettings->KeyBackKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultRightKey"), *PestilenceGameUserSettings->KeyRightKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultCrouchKey"), *PestilenceGameUserSettings->KeyCrouchKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultWalkKey"), *PestilenceGameUserSettings->KeyWalkKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultRollKey"), *PestilenceGameUserSettings->KeyRollKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultInteractKey"), *PestilenceGameUserSettings->KeyInteractKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLeanRightKey"), *PestilenceGameUserSettings->KeyLeanRightKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLeanLeftKey"), *PestilenceGameUserSettings->KeyLeanLeftKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultSaveKey"), *PestilenceGameUserSettings->KeySaveKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLoadKey"), *PestilenceGameUserSettings->KeyLoadKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilityCastKey"), *PestilenceGameUserSettings->KeyAbilityCastKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilityPrepareKey"), *PestilenceGameUserSettings->KeyAbilityPrepareKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility1Key"), *PestilenceGameUserSettings->KeyAbility1Key.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility2Key"), *PestilenceGameUserSettings->KeyAbility2Key.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility3Key"), *PestilenceGameUserSettings->KeyAbility3Key.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility4Key"), *PestilenceGameUserSettings->KeyAbility4Key.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilitySelectNextKey"), *PestilenceGameUserSettings->KeyAbilitySelectNextKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilitySelectPrevKey"), *PestilenceGameUserSettings->KeyAbilitySelectPrevKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultDiaryKey"), *PestilenceGameUserSettings->KeyDiaryKey.ToString(), GGameIni);

			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultCrouchKey"), *PestilenceGameUserSettings->PadCrouchKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultRollKey"), *PestilenceGameUserSettings->PadRollKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultInteractKey"), *PestilenceGameUserSettings->PadInteractKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultLeanRightKey"), *PestilenceGameUserSettings->PadLeanRightKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultLeanLeftKey"), *PestilenceGameUserSettings->PadLeanLeftKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbilityCastKey"), *PestilenceGameUserSettings->PadAbilityCastKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbilityPrepareKey"), *PestilenceGameUserSettings->PadAbilityPrepareKey.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility1Key"), *PestilenceGameUserSettings->PadAbility1Key.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility2Key"), *PestilenceGameUserSettings->PadAbility2Key.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility3Key"), *PestilenceGameUserSettings->PadAbility3Key.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility4Key"), *PestilenceGameUserSettings->PadAbility4Key.ToString(), GGameIni);
			GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultDiaryKey"), *PestilenceGameUserSettings->PadDiaryKey.ToString(), GGameIni);
		}

		if (OptionMenu == EnumOption::ConfirmationExit)
		{
			Exit();
		}
		else 
		{
      SetOptionsFocusInit();
		}
	}
}

void UPestilenceOptionsMenu::OptionsConfirmationNo()
{
	switch (OptionMenu)
	{
  case EnumOption::ConfirmationExit:
    ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
		Exit();
		break;
	case EnumOption::Confirmation:
    ConfirmationPanel->SetVisibility(ESlateVisibility::Hidden);
    SetOptionsFocusInit();

		break;
	default:
		break;
	}
}

void UPestilenceOptionsMenu::OnComboFidelityFXSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	ScaleResolutionSlider->SetValue(100.f);

  FString CombinedString;

  if (SelectedItem == TEXT("Disabled"))
  {
    FidelityFXQuality = 0;
  }
  else if (SelectedItem == TEXT("Quality"))
  {
    CombinedString = TEXT("r.FidelityFX.FSR2.QualityMode 1");
    FidelityFXQuality = 1;
  }
  else if (SelectedItem == TEXT("Balanced"))
  {
    CombinedString = TEXT("r.FidelityFX.FSR2.QualityMode 2");
    FidelityFXQuality = 2;
  }
  else if (SelectedItem == TEXT("Performance"))
  {
    CombinedString = TEXT("r.FidelityFX.FSR2.QualityMode 3");
    FidelityFXQuality = 3;
  }
  else if (SelectedItem == TEXT("Ultra Performance"))
  {
    CombinedString = TEXT("r.FidelityFX.FSR2.QualityMode 4");
    FidelityFXQuality = 4;
  }

  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

  if (!PlayerController) return;

  if (FidelityFXQuality == 0)
  {
    UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("r.FidelityFX.FSR2.Enabled 0"), PlayerController);

		ScaleResolutionSlider->SetLocked(false); 
		FidelityFXSharpnessSlider->SetLocked(true);
  }
  else
  {
    UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("r.FidelityFX.FSR2.Enabled 1"), PlayerController);
    UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), CombinedString, PlayerController);

    ScaleResolutionSlider->SetLocked(true);
    FidelityFXSharpnessSlider->SetLocked(false);
  }
}

void UPestilenceOptionsMenu::SetFidelityFXQuality(int32 _FidelityFXQuality)
{
	FidelityFXComboBox->SetSelectedIndex(_FidelityFXQuality);

  FidelityFXQuality = _FidelityFXQuality;
}

void UPestilenceOptionsMenu::OnSliderFidelityFXSharpnessChanged(float Value)
{
  ScaleResolutionSlider->SetValue(100.f);

  FString Command = FString::Printf(TEXT("r.FidelityFX.FSR2.Sharpness %f"), Value);

  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

  if (!PlayerController) return
  
  UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), Command, PlayerController);
}

void UPestilenceOptionsMenu::OnComboResolutionSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SetResolution(SelectedItem);
}

void UPestilenceOptionsMenu::SetResolution(FString SelectedItem)
{
	int32 FoundIndex = SelectedItem.Find(TEXT("x"));

	if (FoundIndex != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Substring found at index: %d"), FoundIndex);
		FString FirstPart = SelectedItem.Mid(0, FoundIndex);
		FString SecondPart = SelectedItem.Mid(FoundIndex + 1, SelectedItem.Len());
		int32 Value1 = FCString::Atoi(*FirstPart);
		int32 Value2 = FCString::Atoi(*SecondPart);

		FIntPoint Aux;
		Aux.X = Value1;
		Aux.Y = Value2;

		UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
		if (!GameUserSettings) return;
		
		FIntPoint ScreenResolution = GameUserSettings->GetScreenResolution();
		if (ScreenResolution == Aux) return;

		ResolutionX = Aux.X;
		ResolutionY = Aux.Y;

    GameUserSettings->SetScreenResolution(Aux);
    GameUserSettings->ApplyResolutionSettings(true);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Substring not found"));
	}
}

void UPestilenceOptionsMenu::SetComboBoxResolution()
{
	ResolutionComboBox->ClearOptions();
	ResolutionComboBox->RefreshOptions();

	// Get the GameUserSettings object
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();

	TArray<FIntPoint> SupportedResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);

	FString ResolutionString;
	for (const FIntPoint& Resolution : SupportedResolutions)
  {
    ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
		ResolutionComboBox->AddOption(ResolutionString);
		ResolutionComboBox->RefreshOptions();
	}

	//ResolutionComboBox->SetSelectedOption(ResolutionString);
}

void UPestilenceOptionsMenu::OnSliderResolutionScaleChanged(float Value)
{
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		//GameUserSettings->SetResolutionScaleValueEx(Value);
		int32 AuxValue = FMath::RoundToInt(Value);
		FString Command = FString::Printf(TEXT("r.ScreenPercentage %d"), AuxValue);

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), Command, PlayerController);
		}
	}
}

void UPestilenceOptionsMenu::OnSliderDistanceResolutionChanged(float Value)
{
	//FOV
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (PlayerCameraManager)
	{
		PlayerCameraManager->SetFOV(Value);
	}
}

void UPestilenceOptionsMenu::OnComboQualityComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	int32 Value = 0;

	if (GeneralQualityComboBox->GetSelectedOption() != QualityComboBox->GetSelectedOption())
	{
		QualityComboBox->SetSelectedOption(SelectedItem);
	}

	if (SelectedItem == TEXT("Low"))
	{
		TextureComboBox->SetSelectedOption("Low");
		VFXComboBox->SetSelectedOption("Low");
		LevelDetailComboBox->SetSelectedOption("Low");
		PostProccessComboBox->SetSelectedOption("Low");
		GlobalIlluminationComboBox->SetSelectedOption("Low");
		ShadowComboBox->SetSelectedOption("Low");
		AntialiasingComboBox->SetSelectedOption("Low");
	}

	if (SelectedItem == TEXT("Medium"))
	{
		TextureComboBox->SetSelectedOption("Medium");
		VFXComboBox->SetSelectedOption("Medium");
		LevelDetailComboBox->SetSelectedOption("Medium");
		PostProccessComboBox->SetSelectedOption("Medium");
		GlobalIlluminationComboBox->SetSelectedOption("Medium");
		ShadowComboBox->SetSelectedOption("Medium");
		AntialiasingComboBox->SetSelectedOption("Medium");
	}

	if (SelectedItem == TEXT("High"))
	{
		TextureComboBox->SetSelectedOption("High");
		VFXComboBox->SetSelectedOption("High");
		LevelDetailComboBox->SetSelectedOption("High");
		PostProccessComboBox->SetSelectedOption("High");
		GlobalIlluminationComboBox->SetSelectedOption("High");
		ShadowComboBox->SetSelectedOption("High");
		AntialiasingComboBox->SetSelectedOption("High");
	}

	if (SelectedItem == TEXT("Ultra"))
	{
		TextureComboBox->SetSelectedOption("Ultra");
		VFXComboBox->SetSelectedOption("Ultra");
		LevelDetailComboBox->SetSelectedOption("Ultra");
		PostProccessComboBox->SetSelectedOption("Ultra");
		GlobalIlluminationComboBox->SetSelectedOption("Ultra");
		ShadowComboBox->SetSelectedOption("Ultra");
		AntialiasingComboBox->SetSelectedOption("Ultra");
	}
}

void UPestilenceOptionsMenu::OnComboQualityCustomComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	GeneralQualityComboBox->SetSelectedOption(SelectedItem);
}

void UPestilenceOptionsMenu::OnComboFrameRateComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FString CombinedString;

	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		if (SelectedItem == TEXT("Unlimited"))
		{
			CombinedString = TEXT("t.MaxFPS 0");
			//GameUserSettings->SetFrameRateLimit(0.f);
			FPS = 0.f;
		}

		else
		{
			CombinedString = TEXT("t.MaxFPS ") + SelectedItem;
			//GameUserSettings->SetFrameRateLimit(FCString::Atof(*SelectedItem));
			FPS = FCString::Atof(*SelectedItem);
		}
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->ConsoleCommand(CombinedString, true);
	}
}

void UPestilenceOptionsMenu::OnComboAntialiasingComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FString CombinedString;
	int32 Value;

	if (SelectedItem == TEXT("Low"))
	{
		CombinedString = TEXT("sg.AntiAliasingQuality 0");
		Value = 0;
	}

	if (SelectedItem == TEXT("Medium"))
	{
		CombinedString = TEXT("sg.AntiAliasingQuality 1");
		Value = 1;
	}

	if (SelectedItem == TEXT("High"))
	{
		CombinedString = TEXT("sg.AntiAliasingQuality 2");
		Value = 2;
	}

	if (SelectedItem == TEXT("Ultra"))
	{
		CombinedString = TEXT("sg.AntiAliasingQuality 3");
		Value = 3;
	}

	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), CombinedString, PlayerController);

			if (QualityComboBox->GetSelectedOption() != SelectedItem)
			{
				QualityComboBox->SetSelectedOption("Custom");
			}
		}
	}
}

void UPestilenceOptionsMenu::OnComboGlobalIlluminationComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
  FString CombinedString;
  int32 Value;

  if (SelectedItem == TEXT("Low"))
  {
    CombinedString = TEXT("sg.GlobalIlluminationQuality 0");
    Value = 0;
  }

  if (SelectedItem == TEXT("Medium"))
  {
    CombinedString = TEXT("sg.GlobalIlluminationQuality 1");
    Value = 1;
  }

  if (SelectedItem == TEXT("High"))
  {
    CombinedString = TEXT("sg.GlobalIlluminationQuality 2");
    Value = 2;
  }

  if (SelectedItem == TEXT("Ultra"))
  {
    CombinedString = TEXT("sg.GlobalIlluminationQuality 3");
    Value = 3;
  }

  UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
  if (GameUserSettings)
  {
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (PlayerController)
    {
      UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), CombinedString, PlayerController);

      if (QualityComboBox->GetSelectedOption() != SelectedItem)
      {
        QualityComboBox->SetSelectedOption("Custom");
      }
    }
  }
}

void UPestilenceOptionsMenu::OnComboShadowComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
  FString CombinedString;
  int32 Value;

  if (SelectedItem == TEXT("Low"))
  {
    CombinedString = TEXT("sg.ShadowQuality 0");
    Value = 0;
  }

  if (SelectedItem == TEXT("Medium"))
  {
    CombinedString = TEXT("sg.ShadowQuality 1");
    Value = 1;
  }

  if (SelectedItem == TEXT("High"))
  {
    CombinedString = TEXT("sg.ShadowQuality 2");
    Value = 2;
  }

  if (SelectedItem == TEXT("Ultra"))
  {
    CombinedString = TEXT("sg.ShadowQuality 3");
    Value = 3;
  }

  UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
  if (GameUserSettings)
  {
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (PlayerController)
    {
      UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), CombinedString, PlayerController);

      if (QualityComboBox->GetSelectedOption() != SelectedItem)
      {
        QualityComboBox->SetSelectedOption("Custom");
      }
    }
  }
}

void UPestilenceOptionsMenu::OnComboFulscreenComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FString CombinedString;

	if (SelectedItem == TEXT("Fullscreen"))
	{
		UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();;
		if (GameUserSettings)
		{
			GameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
			GameUserSettings->ApplySettings(false);
		}
	}

	if (SelectedItem == TEXT("Window"))
	{
		UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();;
		if (GameUserSettings)
		{
			GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
			GameUserSettings->ApplySettings(false);
		}
	}

	if (SelectedItem == TEXT("Windom Fullscreen"))
	{
		UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();;
		if (GameUserSettings)
		{
			GameUserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
			GameUserSettings->ApplySettings(false);
		}
	}
}

void UPestilenceOptionsMenu::OnComboTextureComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FString CombinedString;
	int32 Value;

	if (SelectedItem == TEXT("Low"))
	{
		CombinedString = TEXT("sg.TextureQuality 0");
		Value = 0;
	}

	if (SelectedItem == TEXT("Medium"))
	{
		CombinedString = TEXT("sg.TextureQuality 1");
		Value = 1;
	}

	if (SelectedItem == TEXT("High"))
	{
		CombinedString = TEXT("sg.TextureQuality 2");
		Value = 2;
	}

	if (SelectedItem == TEXT("Ultra"))
	{
		CombinedString = TEXT("sg.TextureQuality 3");
		Value = 3;
	}

	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), CombinedString, PlayerController);

			if (QualityComboBox->GetSelectedOption() != SelectedItem)
			{
				QualityComboBox->SetSelectedOption("Custom");
			}
		}
	}
}

void UPestilenceOptionsMenu::OnComboVFXSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FString CombinedString;
	int32 Value;

	if (SelectedItem == TEXT("Low"))
	{
		CombinedString = TEXT("sg.EffectsQuality 0");
		Value = 0;
	}

	if (SelectedItem == TEXT("Medium"))
	{
		CombinedString = TEXT("sg.EffectsQuality 1");
		Value = 1;
	}

	if (SelectedItem == TEXT("High"))
	{
		CombinedString = TEXT("sg.EffectsQuality 2");
		Value = 2;
	}

	if (SelectedItem == TEXT("Ultra"))
	{
		CombinedString = TEXT("sg.EffectsQuality 3");
		Value = 3;
	}

	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), CombinedString, PlayerController);

			if (QualityComboBox->GetSelectedOption() != SelectedItem)
			{
				QualityComboBox->SetSelectedOption("Custom");
			}
		}
	}
}

void UPestilenceOptionsMenu::OnComboLevelDetailSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FString CombinedString;
	int32 Value;

	if (SelectedItem == TEXT("Low"))
	{
		CombinedString = TEXT("sg.ViewDistanceQuality 0");
		Value = 0;
	}

	if (SelectedItem == TEXT("Medium"))
	{
		CombinedString = TEXT("sg.ViewDistanceQuality 1");
		Value = 1;
	}

	if (SelectedItem == TEXT("High"))
	{
		CombinedString = TEXT("sg.ViewDistanceQuality 2");
		Value = 2;
	}

	if (SelectedItem == TEXT("Ultra"))
	{
		CombinedString = TEXT("sg.ViewDistanceQuality 3");
		Value = 3;
	}

	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), CombinedString, PlayerController);

			if (QualityComboBox->GetSelectedOption() != SelectedItem)
			{
				QualityComboBox->SetSelectedOption("Custom");
			}
		}
	}
}

void UPestilenceOptionsMenu::OnComboPostProccessSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FString CombinedString;
	int32 Value;

	if (SelectedItem == TEXT("Low"))
	{
		CombinedString = TEXT("sg.PostProcessQuality 0");
		Value = 0;
	}

	if (SelectedItem == TEXT("Medium"))
	{
		CombinedString = TEXT("sg.PostProcessQuality 1");
		Value = 1;
	}

	if (SelectedItem == TEXT("High"))
	{
		CombinedString = TEXT("sg.PostProcessQuality 2");
		Value = 2;
	}

	if (SelectedItem == TEXT("Ultra"))
	{
		CombinedString = TEXT("sg.PostProcessQuality 3");
		Value = 3;
	}

	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (GameUserSettings)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController)
		{
			UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), CombinedString, PlayerController);

			if (QualityComboBox->GetSelectedOption() != SelectedItem)
			{
				QualityComboBox->SetSelectedOption("Custom");
			}
		}
	}
}

void UPestilenceOptionsMenu::OnSliderVolumeMarterChanged(float Value)
{
	if (IsValid(Master))
	{
		Master->Properties.Volume = Value;
	}
}

void UPestilenceOptionsMenu::OnSliderVolumeVFXChanged(float Value)
{
	if (IsValid(VFX))
	{
		VFX->Properties.Volume = Value;
	}
}

void UPestilenceOptionsMenu::OnSliderVolumeVoicesChanged(float Value)
{
	if (IsValid(Voices))
	{
		Voices->Properties.Volume = Value;
	}
}

void UPestilenceOptionsMenu::OnSliderVolumeMusicChanged(float Value)
{
	if (IsValid(Music))
	{
		Music->Properties.Volume = Value;
	}
}

void UPestilenceOptionsMenu::OnCheckBoxSubtitlesChanged(bool IsActivated)
{
	UGameplayStatics::SetSubtitlesEnabled(IsActivated);
}

void UPestilenceOptionsMenu::OnCheckBoxXKeyChanged(bool IsActivated)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->bPestilenceInvertKeyX = IsActivated;
	}
}

void UPestilenceOptionsMenu::OnCheckBoxYKeyChanged(bool IsActivated)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->bPestilenceInvertKeyY = IsActivated;
	}
}

void UPestilenceOptionsMenu::OnCheckBoxXPadChanged(bool IsActivated)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->bPestilenceInvertPadX = IsActivated;
	}
}

void UPestilenceOptionsMenu::OnCheckBoxYPadChanged(bool IsActivated)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->bPestilenceInvertPadY = IsActivated;
	}
}

void UPestilenceOptionsMenu::OnCheckHeadBobbingChanged(bool IsActivated)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->bHeadBobbing = IsActivated;
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			APestilenceCharacter* PestilenceCharacter = Cast<APestilenceCharacter>(PlayerController->GetPawn());
			if (PestilenceCharacter) 
			{
				PestilenceCharacter->GetPestilenceMovementComponent()->EndHeadBobbing();
			}
		}
	}
}

void UPestilenceOptionsMenu::OnSliderSensitivityPadXChanged(float Value)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->PestilenceSensitivityPadX = Value;
	}
}

void UPestilenceOptionsMenu::OnSliderSensitivityPadYChanged(float Value)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->PestilenceSensitivityPadY = Value;
	}
}

void UPestilenceOptionsMenu::OnSliderSensitivityKeyXChanged(float Value)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->PestilenceSensitivityKeyX = Value;
	}
}

void UPestilenceOptionsMenu::OnSliderSensitivityKeyYChanged(float Value)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		PestilenceGameUserSettings->PestilenceSensitivityKeyY = Value;
	}
}

void UPestilenceOptionsMenu::OnSliderAccelerationPadXChanged(float Value)
{
  UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
  if (PestilenceGameUserSettings)
  {
    PestilenceGameUserSettings->PestilenceCameraAccelKeyX = Value;
  }
}

void UPestilenceOptionsMenu::OnSliderAccelerationPadYChanged(float Value)
{
  UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
  if (PestilenceGameUserSettings)
  {
    PestilenceGameUserSettings->PestilenceCameraAccelKeyY = Value;
  }
}

void UPestilenceOptionsMenu::OnSliderAccelerationKeyXChanged(float Value)
{
  UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
  if (PestilenceGameUserSettings)
  {
    PestilenceGameUserSettings->PestilenceCameraAccelPadX = Value;
  }
}

void UPestilenceOptionsMenu::OnSliderAccelerationKeyYChanged(float Value)
{
  UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
  if (PestilenceGameUserSettings)
  {
    PestilenceGameUserSettings->PestilenceCameraAccelPadY = Value;
  }
}

void UPestilenceOptionsMenu::OnCheckBoxPadVibrationChanged(bool IsActivated)
{
	{
		UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
		if (PestilenceGameUserSettings)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController)
			{
				PlayerController->bForceFeedbackEnabled = IsActivated;
			}
		}
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyForwardChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());

	if (PestilenceGameUserSettings)
	{
		if (SelectedKey.Key.IsGamepadKey())
		{
			KeyForwardSelector->SetSelectedKey(PestilenceGameUserSettings->KeyForwardKey);
			return;
		}

		InputMappingContext->UnmapKey(MoveAction, PestilenceGameUserSettings->KeyForwardKey);
		LockpickInputMappingContext->UnmapKey(LockpickMoveAction, PestilenceGameUserSettings->KeyForwardKey);
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(MoveAction, SelectedKey.Key);
		FEnhancedActionKeyMapping& InputLockpick = LockpickInputMappingContext->MapKey(LockpickMoveAction, SelectedKey.Key);

		UInputModifierSwizzleAxis* InputModifierSwizzleAxis = NewObject<UInputModifierSwizzleAxis>();

		UInputTriggerDown* InputTriggerDown = NewObject<UInputTriggerDown>();
		UInputTriggerReleased* InputTriggerReleased = NewObject<UInputTriggerReleased>();

		Input.Modifiers.Add(InputModifierSwizzleAxis);

		Input.Triggers.Add(InputTriggerDown);
		Input.Triggers.Add(InputTriggerReleased);

		UInputModifierSwizzleAxis* LockpickInputModifierSwizzleAxis = NewObject<UInputModifierSwizzleAxis>();

		UInputTriggerDown* LockpickInputTriggerDown = NewObject<UInputTriggerDown>();
		UInputTriggerReleased* LockpickInputTriggerReleased = NewObject<UInputTriggerReleased>();

		InputLockpick.Modifiers.Add(LockpickInputModifierSwizzleAxis);

		InputLockpick.Triggers.Add(LockpickInputTriggerDown);
		InputLockpick.Triggers.Add(LockpickInputTriggerReleased);

		PestilenceGameUserSettings->KeyForwardKey = SelectedKey.Key;
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyBackChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedKey.Key.IsGamepadKey())
		{
			KeyBackSelector->SetSelectedKey(PestilenceGameUserSettings->KeyBackKey);
			return;
		}

		InputMappingContext->UnmapKey(MoveAction, PestilenceGameUserSettings->KeyBackKey);
		LockpickInputMappingContext->UnmapKey(LockpickMoveAction, PestilenceGameUserSettings->KeyBackKey);
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(MoveAction, SelectedKey.Key);
		FEnhancedActionKeyMapping& InputLockpick = LockpickInputMappingContext->MapKey(LockpickMoveAction, SelectedKey.Key);

		UInputModifierSwizzleAxis* InputModifierSwizzleAxis = NewObject<UInputModifierSwizzleAxis>();
		UInputModifierNegate* InputModifierNegate = NewObject<UInputModifierNegate>();

		UInputTriggerDown* InputTriggerDown = NewObject<UInputTriggerDown>();
		UInputTriggerReleased* InputTriggerReleased = NewObject<UInputTriggerReleased>();

		Input.Modifiers.Add(InputModifierSwizzleAxis);
		Input.Modifiers.Add(InputModifierNegate);

		Input.Triggers.Add(InputTriggerDown);
		Input.Triggers.Add(InputTriggerReleased);

		UInputModifierSwizzleAxis* LockpickInputModifierSwizzleAxis = NewObject<UInputModifierSwizzleAxis>();
		UInputModifierNegate* LockpickInputModifierNegate = NewObject<UInputModifierNegate>();

		UInputTriggerDown* LockpickInputTriggerDown = NewObject<UInputTriggerDown>();
		UInputTriggerReleased* LockpickInputTriggerReleased = NewObject<UInputTriggerReleased>();

		InputLockpick.Modifiers.Add(LockpickInputModifierSwizzleAxis);
		InputLockpick.Modifiers.Add(LockpickInputModifierNegate);

		InputLockpick.Triggers.Add(LockpickInputTriggerDown);
		InputLockpick.Triggers.Add(LockpickInputTriggerReleased);

		PestilenceGameUserSettings->KeyBackKey = SelectedKey.Key;
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyRightChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedKey.Key.IsGamepadKey())
		{
			KeyRightSelector->SetSelectedKey(PestilenceGameUserSettings->KeyRightKey);
			return;
		}

		InputMappingContext->UnmapKey(MoveAction, PestilenceGameUserSettings->KeyRightKey);
		LockpickInputMappingContext->UnmapKey(LockpickMoveAction, PestilenceGameUserSettings->KeyRightKey);
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(MoveAction, SelectedKey.Key);
		FEnhancedActionKeyMapping& InputLockpick = LockpickInputMappingContext->MapKey(LockpickMoveAction, SelectedKey.Key);

		UInputTriggerDown* InputTriggerDown = NewObject<UInputTriggerDown>();
		UInputTriggerReleased* InputTriggerReleased = NewObject<UInputTriggerReleased>();

		Input.Triggers.Add(InputTriggerDown);
		Input.Triggers.Add(InputTriggerReleased);

		UInputTriggerDown* LockpickInputTriggerDown = NewObject<UInputTriggerDown>();
		UInputTriggerReleased* LockpickInputTriggerReleased = NewObject<UInputTriggerReleased>();

		InputLockpick.Triggers.Add(LockpickInputTriggerDown);
		InputLockpick.Triggers.Add(LockpickInputTriggerReleased);

		PestilenceGameUserSettings->KeyRightKey = SelectedKey.Key;
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyLeftChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedKey.Key.IsGamepadKey())
		{
			KeyLeftSelector->SetSelectedKey(PestilenceGameUserSettings->KeyLeftKey);
			return;
		}

		InputMappingContext->UnmapKey(MoveAction, PestilenceGameUserSettings->KeyLeftKey);
		LockpickInputMappingContext->UnmapKey(LockpickMoveAction, PestilenceGameUserSettings->KeyLeftKey);
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(MoveAction, SelectedKey.Key);
		FEnhancedActionKeyMapping& InputLockpick = LockpickInputMappingContext->MapKey(LockpickMoveAction, SelectedKey.Key);

		UInputModifierNegate* InputModifierNegate = NewObject<UInputModifierNegate>();

		UInputTriggerDown* InputTriggerDown = NewObject<UInputTriggerDown>();
		UInputTriggerReleased* InputTriggerReleased = NewObject<UInputTriggerReleased>();

		Input.Modifiers.Add(InputModifierNegate);

		Input.Triggers.Add(InputTriggerDown);
		Input.Triggers.Add(InputTriggerReleased);

		UInputModifierNegate* LockpickInputModifierNegate = NewObject<UInputModifierNegate>();

		UInputTriggerDown* LockpickInputTriggerDown = NewObject<UInputTriggerDown>();
		UInputTriggerReleased* LockpickInputTriggerReleased = NewObject<UInputTriggerReleased>();

		InputLockpick.Modifiers.Add(LockpickInputModifierNegate);

		InputLockpick.Triggers.Add(LockpickInputTriggerDown);
		InputLockpick.Triggers.Add(LockpickInputTriggerReleased);

		PestilenceGameUserSettings->KeyLeftKey = SelectedKey.Key;
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyCrouchChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(CrouchAction, PestilenceGameUserSettings->KeyCrouchKey);
		PestilenceGameUserSettings->KeyCrouchKey = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(CrouchAction, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyWalkChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(WalkAction, PestilenceGameUserSettings->KeyWalkKey);
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(WalkAction, SelectedKey.Key);

		UInputTriggerPressed* InputTriggerPressed = NewObject<UInputTriggerPressed>();
		UInputTriggerReleased* InputTriggerReleased = NewObject<UInputTriggerReleased>();

		Input.Triggers.Add(InputTriggerPressed);
		Input.Triggers.Add(InputTriggerReleased);

		PestilenceGameUserSettings->KeyWalkKey = SelectedKey.Key;
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyRollChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(RollAction, PestilenceGameUserSettings->KeyRollKey);
		PestilenceGameUserSettings->KeyRollKey = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(RollAction, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyInteractChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(InteractAction, PestilenceGameUserSettings->KeyInteractKey);
		InputMappingContext->UnmapKey(InteractUIAction, PestilenceGameUserSettings->KeyInteractKey);
		PestilenceGameUserSettings->KeyInteractKey = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(InteractAction, SelectedKey.Key);
		FEnhancedActionKeyMapping& Input2 = InputMappingContext->MapKey(InteractUIAction, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyLeanRightChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(LeanRightAction, PestilenceGameUserSettings->KeyLeanRightKey);
		PestilenceGameUserSettings->KeyLeanRightKey = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(LeanRightAction, SelectedKey.Key);

		UInputTriggerPressed* InputTriggerPressed = NewObject<UInputTriggerPressed>();
		UInputTriggerReleased* InputTriggerReleased = NewObject<UInputTriggerReleased>();

		Input.Triggers.Add(InputTriggerPressed);
		Input.Triggers.Add(InputTriggerReleased);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyLeanLeftChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(LeanLeftAction, PestilenceGameUserSettings->KeyLeanLeftKey);
		PestilenceGameUserSettings->KeyLeanLeftKey = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(LeanLeftAction, SelectedKey.Key);

		UInputTriggerPressed* InputTriggerPressed = NewObject<UInputTriggerPressed>();
		UInputTriggerReleased* InputTriggerReleased = NewObject<UInputTriggerReleased>();

		Input.Triggers.Add(InputTriggerPressed);
		Input.Triggers.Add(InputTriggerReleased);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeySaveChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(SaveAction, PestilenceGameUserSettings->KeySaveKey);
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(SaveAction, SelectedKey.Key);

		PestilenceGameUserSettings->KeySaveKey = SelectedKey.Key;
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyLoadChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(LoadAction, PestilenceGameUserSettings->KeyLoadKey);
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(LoadAction, SelectedKey.Key);

		PestilenceGameUserSettings->KeyLoadKey = SelectedKey.Key;
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyAbilityCastChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(AbilityCastAction, PestilenceGameUserSettings->KeyAbilityCastKey);
		PossessionInputMappingContext->UnmapKey(AbilityCastAction, PestilenceGameUserSettings->KeyAbilityCastKey);
		PestilenceGameUserSettings->KeyAbilityCastKey = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilityCastAction, SelectedKey.Key);
		FEnhancedActionKeyMapping& InputPossession = PossessionInputMappingContext->MapKey(AbilityCastAction, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyAbilityPrepareChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(AbilityPrepareAction, PestilenceGameUserSettings->KeyAbilityPrepareKey);
		PossessionInputMappingContext->UnmapKey(AbilityPrepareAction, PestilenceGameUserSettings->KeyAbilityPrepareKey);
		LockpickInputMappingContext->UnmapKey(AbilityPrepareAction, PestilenceGameUserSettings->KeyAbilityPrepareKey);
		PestilenceGameUserSettings->KeyAbilityPrepareKey = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilityPrepareAction, SelectedKey.Key);
		FEnhancedActionKeyMapping& InputPossession = PossessionInputMappingContext->MapKey(AbilityPrepareAction, SelectedKey.Key);
		FEnhancedActionKeyMapping& InputLockpick = LockpickInputMappingContext->MapKey(AbilityPrepareAction, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyAbility1Change(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(AbilitySelect1Action, PestilenceGameUserSettings->KeyAbility1Key);
		PestilenceGameUserSettings->KeyAbility1Key = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelect1Action, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyAbility2Change(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(AbilitySelect2Action, PestilenceGameUserSettings->KeyAbility2Key);
		PestilenceGameUserSettings->KeyAbility2Key = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelect2Action, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyAbility3Change(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(AbilitySelect3Action, PestilenceGameUserSettings->KeyAbility3Key);
		PestilenceGameUserSettings->KeyAbility3Key = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelect3Action, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyAbility4Change(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(AbilitySelect4Action, PestilenceGameUserSettings->KeyAbility4Key);
		PestilenceGameUserSettings->KeyAbility4Key = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelect4Action, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyAbilitySelectNextChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(AbilitySelectNextAction, PestilenceGameUserSettings->KeyAbilitySelectNextKey);
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelectNextAction, SelectedKey.Key);

		PestilenceGameUserSettings->KeyAbilitySelectNextKey = SelectedKey.Key;
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyAbilitySelectPrevChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(AbilitySelectPrevAction, PestilenceGameUserSettings->KeyAbilitySelectPrevKey);
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelectPrevAction, SelectedKey.Key);

		PestilenceGameUserSettings->KeyAbilitySelectPrevKey = SelectedKey.Key;
	}
}

void UPestilenceOptionsMenu::OnSelectorKeyDiaryChange(FInputChord SelectedKey)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		InputMappingContext->UnmapKey(DiaryAction, PestilenceGameUserSettings->KeyDiaryKey);
		LockpickInputMappingContext->UnmapKey(DiaryAction, PestilenceGameUserSettings->KeyDiaryKey);
		PestilenceGameUserSettings->KeyDiaryKey = SelectedKey.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(DiaryAction, SelectedKey.Key);
		FEnhancedActionKeyMapping& InputLockpick = LockpickInputMappingContext->MapKey(DiaryAction, SelectedKey.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadCrouchChange(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadCrouchSelector->SetSelectedKey(PestilenceGameUserSettings->PadCrouchKey);
			return;
		}
		InputMappingContext->UnmapKey(CrouchAction, PestilenceGameUserSettings->PadCrouchKey);
		PestilenceGameUserSettings->PadCrouchKey = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(CrouchAction, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadRollChange(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadRollSelector->SetSelectedKey(PestilenceGameUserSettings->PadRollKey);
			return;
		}
		InputMappingContext->UnmapKey(RollAction, PestilenceGameUserSettings->PadRollKey);
		PestilenceGameUserSettings->PadRollKey = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(RollAction, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadInteractChange(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadInteractSelector->SetSelectedKey(PestilenceGameUserSettings->PadInteractKey);
			return;
		}
		InputMappingContext->UnmapKey(InteractAction, PestilenceGameUserSettings->PadInteractKey);
		InputMappingContext->UnmapKey(InteractUIAction, PestilenceGameUserSettings->PadInteractKey);
		PestilenceGameUserSettings->PadInteractKey = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(InteractAction, SelectedPad.Key);
		FEnhancedActionKeyMapping& Input2 = InputMappingContext->MapKey(InteractUIAction, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadLeanRightChange(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadLeanRightSelector->SetSelectedKey(PestilenceGameUserSettings->PadLeanRightKey);
			return;
		}
		InputMappingContext->UnmapKey(LeanRightAction, PestilenceGameUserSettings->PadLeanRightKey);
		PestilenceGameUserSettings->PadLeanRightKey = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(LeanRightAction, SelectedPad.Key);

		UInputTriggerPressed* InputTriggerPressed = NewObject<UInputTriggerPressed>();
		UInputTriggerReleased* InputTriggerReleased = NewObject<UInputTriggerReleased>();

		Input.Triggers.Add(InputTriggerPressed);
		Input.Triggers.Add(InputTriggerReleased);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadLeanLeftChange(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadLeanLeftSelector->SetSelectedKey(PestilenceGameUserSettings->PadLeanLeftKey);
			return;
		}
		InputMappingContext->UnmapKey(LeanLeftAction, PestilenceGameUserSettings->PadLeanLeftKey);
		PestilenceGameUserSettings->PadLeanLeftKey = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(LeanLeftAction, SelectedPad.Key);

		UInputTriggerPressed* InputTriggerPressed = NewObject<UInputTriggerPressed>();
		UInputTriggerReleased* InputTriggerReleased = NewObject<UInputTriggerReleased>();

		Input.Triggers.Add(InputTriggerPressed);
		Input.Triggers.Add(InputTriggerReleased);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadAbilityCastChange(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadAbilityCastSelector->SetSelectedKey(PestilenceGameUserSettings->PadAbilityCastKey);
			return;
		}
		InputMappingContext->UnmapKey(AbilityCastAction, PestilenceGameUserSettings->PadAbilityCastKey);
		PossessionInputMappingContext->UnmapKey(AbilityCastAction, PestilenceGameUserSettings->PadAbilityCastKey);
		PestilenceGameUserSettings->PadAbilityCastKey = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilityCastAction, SelectedPad.Key);
		FEnhancedActionKeyMapping& InputPossession = PossessionInputMappingContext->MapKey(AbilityCastAction, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadAbilityPrepareChange(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadAbilityPrepareSelector->SetSelectedKey(PestilenceGameUserSettings->PadAbilityPrepareKey);
		}
		InputMappingContext->UnmapKey(AbilityPrepareAction, PestilenceGameUserSettings->PadAbilityPrepareKey);
		PossessionInputMappingContext->UnmapKey(AbilityPrepareAction, PestilenceGameUserSettings->PadAbilityPrepareKey);
		LockpickInputMappingContext->UnmapKey(AbilityPrepareAction, PestilenceGameUserSettings->PadAbilityPrepareKey);

		PestilenceGameUserSettings->PadAbilityPrepareKey = SelectedPad.Key;

		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilityPrepareAction, SelectedPad.Key);
		FEnhancedActionKeyMapping& InputPossession = PossessionInputMappingContext->MapKey(AbilityPrepareAction, SelectedPad.Key);
		FEnhancedActionKeyMapping& InputLockpick = LockpickInputMappingContext->MapKey(AbilityPrepareAction, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadAbility1Change(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadAbility1Selector->SetSelectedKey(PestilenceGameUserSettings->PadAbility1Key);
		}
		InputMappingContext->UnmapKey(AbilitySelect1Action, PestilenceGameUserSettings->PadAbility1Key);
		PestilenceGameUserSettings->PadAbility1Key = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelect1Action, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadAbility2Change(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if(SelectedPad.Key.IsMouseButton())
		{
			PadAbility2Selector->SetSelectedKey(PestilenceGameUserSettings->PadAbility2Key);
		}
		InputMappingContext->UnmapKey(AbilitySelect2Action, PestilenceGameUserSettings->PadAbility2Key);
		PestilenceGameUserSettings->PadAbility2Key = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelect2Action, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadAbility3Change(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadAbility3Selector->SetSelectedKey(PestilenceGameUserSettings->PadAbility3Key);
		}
		InputMappingContext->UnmapKey(AbilitySelect3Action, PestilenceGameUserSettings->PadAbility3Key);
		PestilenceGameUserSettings->PadAbility3Key = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelect3Action, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadAbility4Change(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			PadAbility4Selector->SetSelectedKey(PestilenceGameUserSettings->PadAbility4Key);
		}
		InputMappingContext->UnmapKey(AbilitySelect4Action, PestilenceGameUserSettings->PadAbility4Key);
		PestilenceGameUserSettings->PadAbility4Key = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(AbilitySelect4Action, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::OnSelectorPadDiaryChange(FInputChord SelectedPad)
{
	UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (PestilenceGameUserSettings)
	{
		if (SelectedPad.Key.IsMouseButton())
		{
			KeyDiarySelector->SetSelectedKey(PestilenceGameUserSettings->PadDiaryKey);
		}
		InputMappingContext->UnmapKey(DiaryAction, PestilenceGameUserSettings->PadDiaryKey);
		LockpickInputMappingContext->UnmapKey(DiaryAction, PestilenceGameUserSettings->PadDiaryKey);
		PestilenceGameUserSettings->PadDiaryKey = SelectedPad.Key;
		FEnhancedActionKeyMapping& Input = InputMappingContext->MapKey(DiaryAction, SelectedPad.Key);
		FEnhancedActionKeyMapping& InputLockpick = LockpickInputMappingContext->MapKey(DiaryAction, SelectedPad.Key);
	}
}

void UPestilenceOptionsMenu::Click(UUserWidget* Button)
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

TArray<UWidget*> UPestilenceOptionsMenu::GetAllWidgetsRecursively()
{
	TArray<UWidget*> Children;
	this->WidgetTree->GetAllWidgets(Children);
	return(Children);
}
