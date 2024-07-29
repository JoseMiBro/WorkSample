// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Core/PestilenceGameUserSettings.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedActionKeyMapping.h"

void UPestilenceGameUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultForwardKey"), *KeyDefaultForwardKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLeftKey"), *KeyDefaultLeftKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultBackKey"), *KeyDefaultBackKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultRightKey"), *KeyDefaultRightKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultCrouchKey"), *KeyDefaultCrouchKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultWalkKey"), *KeyDefaultWalkKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultRollKey"), *KeyDefaultRollKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultInteractKey"), *KeyDefaultInteractKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLeanRightKey"), *KeyDefaultLeanRightKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLeanLeftKey"), *KeyDefaultLeanLeftKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultSaveKey"), *KeyDefaultSaveKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultLoadKey"), *KeyDefaultLoadKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilityCastKey"), *KeyDefaultAbilityCastKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilityPrepareKey"), *KeyDefaultAbilityPrepareKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility1Key"), *KeyDefaultAbility1Key.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility2Key"), *KeyDefaultAbility2Key.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility3Key"), *KeyDefaultAbility3Key.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbility4Key"), *KeyDefaultAbility4Key.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilitySelectNextKey"), *KeyDefaultAbilitySelectNextKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultAbilitySelectPrevKey"), *KeyDefaultAbilitySelectPrevKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("KeyDefaultDiaryKey"), *KeyDefaultDiaryKey.ToString(), GGameIni);
	//
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultCrouchKey"), *PadDefaultCrouchKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultRollKey"), *PadDefaultRollKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultInteractKey"), *PadDefaultInteractKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultLeanRightKey"), *PadDefaultLeanRightKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultLeanLeftKey"), *PadDefaultLeanLeftKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbilityCastKey"), *PadDefaultAbilityCastKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbilityPrepareKey"), *PadDefaultAbilityPrepareKey.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility1Key"), *EKeys::Gamepad_DPad_Up.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility2Key"), *EKeys::Gamepad_DPad_Down.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility3Key"), *EKeys::Gamepad_DPad_Right.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultAbility4Key"), *EKeys::Gamepad_DPad_Left.ToString(), GGameIni);
	//GConfig->SetString(TEXT("PestilenceAuxInputs"), TEXT("PadDefaultDiaryKey"), *PadDefaultDiaryKey.ToString(), GGameIni);
}

void UPestilenceGameUserSettings::SaveSettings()
{
	Super::SaveSettings();
}
