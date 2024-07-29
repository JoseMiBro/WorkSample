#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RadialSlider.h"

#include "Pestilence/Characters/PestilenceCharacter.h"

#include "LockpickWidget.generated.h"

class UAudioComponent;
class USoundBase;
class AIA_SwingDoor;


//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLockpickSuccess);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLockpickFailure);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInit);

//USTRUCT(BlueprintType)
//struct FPuzzleStruct
//{
//  GENERATED_BODY()
//
//public:
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    FLinearColor L_ColorHitINSlider;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    FLinearColor L_ColorHitOUTSlider;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    FLinearColor R_ColorHitINSlider;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    FLinearColor R_ColorHitOUTSlider;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    float L_TimeStartedStep;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    float R_TimeStartedStep;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    float L_TimeCompletedStep;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    float R_TimeCompletedStep;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    float L_HitStartSlider;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    float L_HitEndSlider;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    float R_HitStartSlider;
//
//  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
//    float R_HitEndSlider;
//};

UCLASS()
class PESTILENCE_API ULockpickWidget : public UUserWidget
{
    GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadOnly)
    TObjectPtr<AIA_SwingDoor> Door;
  UPROPERTY(BlueprintAssignable, Category = "Lockpicking")
    FOnInit ReStart;

    //UPROPERTY(BlueprintAssignable, Category = "Lockpicking")
    //FOnLockpickSuccess OnLockpickSuccess;
    //
    //UPROPERTY(BlueprintAssignable, Category = "Lockpicking")
    //FOnLockpickFailure OnLockpickFailure;

    //UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    //  URadialSlider* L_BasePuzzleSlider;
    //
    //UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    //  URadialSlider* R_BasePuzzleSlider;
    //
    //UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    //  URadialSlider* L_HitPuzzleSlider;
    //
    //UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    //  URadialSlider* R_HitPuzzleSlider;

    //UPROPERTY(EditAnywhere, Category = "PuzzleSlider")
    //  int IndexPuzzleCongigurations = 0;

    //UPROPERTY(EditAnywhere, Category = "PuzzleSlider")
    //  TArray<FPuzzleStruct> PuzzleCongigurations;

    //UPROPERTY(BlueprintReadWrite, Category = "PuzzleSlider")
    //  float L_PuzzleActualProgress;
    //
    //UPROPERTY(BlueprintReadWrite, Category = "PuzzleSlider")
    //  float R_PuzzleActualProgress;

    //PsedoStruct
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  FLinearColor L_ColorHitINSlider = FLinearColor::Red;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  FLinearColor L_ColorHitOUTSlider = FLinearColor::Black;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  FLinearColor R_ColorHitINSlider = FLinearColor::Red;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  FLinearColor R_ColorHitOUTSlider = FLinearColor::Black;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  float L_TimeStartedStep = 0.1f;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  float R_TimeStartedStep = 0.01f;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  float L_TimeCompletedStep = 0.01f;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  float R_TimeCompletedStep = 0.1f;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  float L_HitStartSlider = 230.f;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  float L_HitEndSlider = 300.f;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  float R_HitStartSlider = 90.f;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //  float R_HitEndSlider = 150.f;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //TObjectPtr<USoundBase> LeftAssetSound;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //TObjectPtr<USoundBase> RightAssetSound;
    //
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PuzzleSlider")
    //TObjectPtr<USoundBase> CorrectSound;
    //
    //bool bIsInMinigame = false;

private:
  //UPROPERTY()
  //  FTimerHandle L_StartedPuzzleHandle;
  //
  //UPROPERTY()
  //  FTimerHandle L_CompletedPuzzleHandle;
  //
  //UPROPERTY()
  //  FTimerHandle R_StartedPuzzleHandle;
  //
  //UPROPERTY()
  //  FTimerHandle R_CompletedPuzzleHandle;
  //
  //UPROPERTY()
  //TObjectPtr<UAudioComponent> LeftAssetAudioComponent;
  //
  //UPROPERTY()
  //TObjectPtr<UAudioComponent> RightAssetAudioComponent;
  //
  //bool bLeftInSlider = false;
  //bool bRightInSlider = false;
protected:
  virtual void NativeConstruct() override;

  UFUNCTION(BlueprintImplementableEvent)
    void RestartMiniGame();

public:
  virtual bool Initialize() override;
  virtual void NativeOnInitialized();
  virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
  
    //UFUNCTION(BlueprintCallable)
    //void LockPickSuccess();
    //
    //UFUNCTION(BlueprintCallable)
    //void LockPickFailure();
    //
    //UFUNCTION(BlueprintCallable)
    //  void L_StartedPuzzle();
    //
    //UFUNCTION(BlueprintCallable)
    //  void L_CompletedPuzzle();
    //
    //UFUNCTION(BlueprintCallable)
    //  void R_StartedPuzzle();
    //
    //UFUNCTION(BlueprintCallable)
    //  void R_CompletedPuzzle();
    //
    //UFUNCTION(BlueprintCallable)
    //  void PuzzleFailExit();

    //UFUNCTION()
    //  void L_StartedFuntionPuzzle();
    //
    //UFUNCTION()
    //  void L_CompletedFuntionPuzzle();
    //
    //UFUNCTION()
    //  void R_StartedFuntionPuzzle();
    //
    //UFUNCTION()
    //  void R_CompletedFuntionPuzzle();
    //
    //UFUNCTION()
    //  float UpdateBar(bool _incremental, float _progress,URadialSlider* _Slider_);

};
