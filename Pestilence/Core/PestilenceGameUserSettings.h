// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "PestilenceGameUserSettings.generated.h"

//struct FEnhancedActionKeyMapping;

UCLASS(config = PestilenceGameUserSettings, configdonotcheckdefaults)
class PESTILENCE_API UPestilenceGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bPestilenceInvertKeyX;
	UPROPERTY()
	bool bPestilenceInvertKeyY;
	UPROPERTY()
	bool bPestilenceInvertPadX;
	UPROPERTY()
	bool bPestilenceInvertPadY;
	UPROPERTY()
	float PestilenceSensitivityKeyX;
	UPROPERTY()
	float PestilenceSensitivityKeyY;
	UPROPERTY()
	float PestilenceSensitivityPadX;
	UPROPERTY()
	float PestilenceSensitivityPadY;
	
	//Camera acceleration magnitude: 0=No accel, 1=Double speed as max., 2=Triple, etc
	UPROPERTY()
	float PestilenceCameraAccelKeyX = 1.0f;
	UPROPERTY()
	float PestilenceCameraAccelKeyY = 1.0f;
	UPROPERTY()
	float PestilenceCameraAccelPadX = 1.0f;
	UPROPERTY()
	float PestilenceCameraAccelPadY = 1.0f;
	
	UPROPERTY()
	bool bIsVibrationPad;
	UPROPERTY()
	bool bHeadBobbing;
	UPROPERTY()
	bool bHaveSave;
	
	FKey KeyForwardKey;
	FKey KeyLeftKey;
	FKey KeyBackKey;
	FKey KeyRightKey;

	FKey KeyCrouchKey;
	FKey KeyWalkKey;
	FKey KeyRollKey;

	FKey KeyInteractKey;

	FKey KeyLeanRightKey;
	FKey KeyLeanLeftKey;

	FKey KeySaveKey;
	FKey KeyLoadKey;

	FKey KeyAbilityCastKey;
	FKey KeyAbilityPrepareKey;

	FKey KeyAbility1Key;
	FKey KeyAbility2Key;
	FKey KeyAbility3Key;
	FKey KeyAbility4Key;

	FKey KeyAbilitySelectNextKey;
	FKey KeyAbilitySelectPrevKey;

	FKey KeyDiaryKey;

	//Save
	FString KeyDefaultForward;
	FString KeyDefaultLeft;
	FString KeyDefaultBack;
	FString KeyDefaultRight;

	FString KeyDefaultCrouch;
	FString KeyDefaultWalk;
	FString KeyDefaultRoll;

	FString KeyDefaultInteract;

	FString KeyDefaultLeanRight;
	FString KeyDefaultLeanLeft;

	FString KeyDefaultSave;
	FString KeyDefaultLoad;

	FString KeyDefaultAbilityCast;
	FString KeyDefaultAbilityPrepare;

	FString KeyDefaultAbility1;
	FString KeyDefaultAbility2;
	FString KeyDefaultAbility3;
	FString KeyDefaultAbility4;

	FString KeyDefaultAbilitySelectNext;
	FString KeyDefaultAbilitySelectPrev;

	FString KeyDefaultDiary;

	//GamePad
	FKey PadCrouchKey;
	FKey PadRollKey;

	FKey PadInteractKey;
	
	FKey PadLeanRightKey;
	FKey PadLeanLeftKey;
	
	FKey PadAbilityCastKey;
	FKey PadAbilityPrepareKey;
	
	FKey PadAbility1Key;
	FKey PadAbility2Key;
	FKey PadAbility3Key;
	FKey PadAbility4Key;
	
	FKey PadDiaryKey;

	//Save
	FString PadDefaultCrouch;
	FString PadDefaultRoll;
	
	FString PadDefaultInteract;
	
	FString PadDefaultLeanRight;
	FString PadDefaultLeanLeft;
	
	FString PadDefaultAbilityCast;
	FString PadDefaultAbilityPrepare;
	
	FString PadDefaultAbility1;
	FString PadDefaultAbility2;
	FString PadDefaultAbility3;
	FString PadDefaultAbility4;
	
	FString PadDefaultDiary;

public:
	virtual void LoadSettings(bool bForceReload = false) override;
	virtual void SaveSettings() override;
};
