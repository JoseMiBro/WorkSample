// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Characters/Input/PestilenceInputModifier.h"

#include <string>

#include "Pestilence/Core/PestilenceGameUserSettings.h"
//#include "GameFramework/InputSettings.h"


FInputActionValue UPestilenceInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
  ////Esto hay que cambiarlo
  //UInputSettings* InputSettings = UInputSettings::StaticClass()->GetDefaultObject<UInputSettings>();
  //if (InputSettings->bCaptureMouseOnLaunch)
  //{
  //  CurrentValue.Get<FVector>() = CurrentValue.Get<FVector>() * FVector(bKeyX ? -1.f : 1.f, bKeyY ? -1.f : 1.f, 1.f);
  //  CurrentValue.Get<FVector>() = CurrentValue.Get<FVector>() * FVector(SensitivityKeyX, SensitivityKeyY, 1.f);
  //}
  //
  //else
  //{
  //  CurrentValue.Get<FVector>() = CurrentValue.Get<FVector>()* FVector(bPadX ? -1.f : 1.f, bPadY ? -1.f : 1.f, 1.f);
  //  CurrentValue.Get<FVector>() = CurrentValue.Get<FVector>()* FVector(SensitivityPadX, SensitivityPadY, 1.f);
  //}
  
  //Temporal
  UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
  
  if (GameUserSettings)
  {
    UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
    if (PestilenceGameUserSettings)
    {
      CurrentValue = CurrentValue.Get<FVector2D>() * FVector2D(PestilenceGameUserSettings->bPestilenceInvertKeyX ? -1.f : 1.f, PestilenceGameUserSettings->bPestilenceInvertKeyY ? -1.f : 1.f);
      CurrentValue = CurrentValue.Get<FVector2D>() * FVector2D(PestilenceGameUserSettings->PestilenceSensitivityKeyX, PestilenceGameUserSettings->PestilenceSensitivityKeyY);
      //Check if value is enough to accelerate
      if (CurrentValue.GetMagnitudeSq() >= AccelerationDeadzone * AccelerationDeadzone)
      {
        //Increase acceleration
        AccelerationX = AccelerationX + DeltaTime * (1/SecondsToMaxSpeed);
        AccelerationY = AccelerationY + DeltaTime * (1/SecondsToMaxSpeed);
        if (AccelerationX > 1)
        {
          AccelerationX = 1;
        }
        if (AccelerationY > 1)
        {
          AccelerationY = 1;
        }

        //Apply User Settings modifier
        float tempAccX = AccelerationX * PestilenceGameUserSettings->PestilenceCameraAccelKeyX;
        float tempAccY = AccelerationY * PestilenceGameUserSettings->PestilenceCameraAccelKeyY;

        //Increase camera speed accordingly
        CurrentValue = CurrentValue.Get<FVector2D>() + FVector2D(CurrentValue.Get<FVector2d>().Component(0) * tempAccX, CurrentValue.Get<FVector2d>().Component(1) * tempAccY);
      } else
      {
        //Decrease acceleration
        AccelerationX = 0;
        AccelerationY = 0;
      }
    }
  }

  return CurrentValue;
}

FInputActionValue UPestilenceInputControllerModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
  UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
  if (GameUserSettings)
  {
    UPestilenceGameUserSettings* PestilenceGameUserSettings = Cast<UPestilenceGameUserSettings>(UGameUserSettings::GetGameUserSettings());
    if (PestilenceGameUserSettings)
    {
      CurrentValue = CurrentValue.Get<FVector2D>() * FVector2D(PestilenceGameUserSettings->bPestilenceInvertPadX ? -1.f : 1.f, PestilenceGameUserSettings->bPestilenceInvertPadY ? -1.f : 1.f);
      CurrentValue = CurrentValue.Get<FVector2D>() * FVector2D(PestilenceGameUserSettings->PestilenceSensitivityPadX, PestilenceGameUserSettings->PestilenceSensitivityPadY);
      //Check if value is enough to accelerate
      if (CurrentValue.GetMagnitudeSq() >= AccelerationDeadzone * AccelerationDeadzone)
      {
        //Increase acceleration
        AccelerationX = AccelerationX + DeltaTime * (1/SecondsToMaxSpeed);
        AccelerationY = AccelerationY + DeltaTime * (1/SecondsToMaxSpeed);
        if (AccelerationX > 1)
        {
          AccelerationX = 1;
        }
        if (AccelerationY > 1)
        {
          AccelerationY = 1;
        }

        //Apply User Settings modifier
        float tempAccX = AccelerationX * PestilenceGameUserSettings->PestilenceCameraAccelPadX;
        float tempAccY = AccelerationY * PestilenceGameUserSettings->PestilenceCameraAccelPadY;

        //Increase camera speed accordingly
        CurrentValue = CurrentValue.Get<FVector2D>() + FVector2D(CurrentValue.Get<FVector2d>().Component(0) * tempAccX, CurrentValue.Get<FVector2d>().Component(1) * tempAccY);
      } else
      {
        //Decrease acceleration

        AccelerationX = 0;
        AccelerationY = 0;

        /*accelerationX = accelerationX - DeltaTime * (1/SecondsToBaseSpeed);
        accelerationY = accelerationY - DeltaTime * (1/SecondsToBaseSpeed);
        if (accelerationX < 0)
        {
          accelerationX = 0;
        }
        if (accelerationY < 0)
        {
          accelerationY = 0;
        }*/
      }
    }
  }

  return CurrentValue;
}
