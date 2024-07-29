#include "Pestilence/Widgets/LockpickWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


void ULockpickWidget::NativeConstruct()
{
  Super::NativeConstruct();
}

bool ULockpickWidget::Initialize()
{
  return Super::Initialize();
}

void ULockpickWidget::NativeOnInitialized()
{
  Super::NativeOnInitialized();


  ReStart.AddDynamic(this, &ULockpickWidget::RestartMiniGame);

  //L_HitPuzzleSlider->SliderHandleStartAngle = L_HitStartSlider;
  //L_HitPuzzleSlider->SliderHandleEndAngle = L_HitEndSlider;
  //R_HitPuzzleSlider->SliderHandleStartAngle = R_HitStartSlider;
  //R_HitPuzzleSlider->SliderHandleEndAngle = R_HitEndSlider;
  //
  //LeftAssetAudioComponent = UGameplayStatics::CreateSound2D(this, LeftAssetSound, 1.f, 1.f, 0.f, nullptr, false, false);
  //RightAssetAudioComponent = UGameplayStatics::CreateSound2D(this, RightAssetSound, 1.f, 1.f, 0.f, nullptr, false, false);
}

void ULockpickWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);
  //bool bLeftCorrect = L_HitPuzzleSlider->SliderHandleStartAngle <= L_PuzzleActualProgress * 3.6f &&
  //  L_PuzzleActualProgress * 3.6f <= L_HitPuzzleSlider->SliderHandleEndAngle;
  //bool bRightCorrect = R_HitPuzzleSlider->SliderHandleStartAngle <= R_PuzzleActualProgress * 3.6f &&
  //  R_PuzzleActualProgress * 3.6f <= R_HitPuzzleSlider->SliderHandleEndAngle;
  //
  //if (!bLeftInSlider && bLeftCorrect)
  //{
  //  bLeftInSlider = true;
  //  L_HitPuzzleSlider->SetSliderBarColor(L_ColorHitINSlider);
  //  UGameplayStatics::PlaySound2D(this, CorrectSound);
  //}
  //else if (bLeftInSlider && !bLeftCorrect)
  //{
  //  bLeftInSlider = false;
  //  L_HitPuzzleSlider->SetSliderBarColor(L_ColorHitOUTSlider);
  //}
  //
  //if (!bRightInSlider && bRightCorrect)
  //{
  //  bRightInSlider = true;
  //  R_HitPuzzleSlider->SetSliderBarColor(R_ColorHitINSlider);
  //  UGameplayStatics::PlaySound2D(this, CorrectSound);
  //}
  //else if (bRightInSlider && !bRightCorrect)
  //{
  //  bRightInSlider = false;
  //  R_HitPuzzleSlider->SetSliderBarColor(R_ColorHitOUTSlider);
  //}
  //
  //if (bLeftInSlider && bRightInSlider)
  //{
  //  LockPickSuccess();
  //}
}

//void ULockpickWidget::LockPickSuccess()
//{
//  LeftAssetAudioComponent->Stop();
//  RightAssetAudioComponent->Stop();
//
//  GetWorld()->GetTimerManager().ClearTimer(L_StartedPuzzleHandle);
//  GetWorld()->GetTimerManager().ClearTimer(L_CompletedPuzzleHandle);
//
//  OnLockpickSuccess.Broadcast();
//}
//
//void ULockpickWidget::LockPickFailure()
//{
//  GetWorld()->GetTimerManager().ClearTimer(L_StartedPuzzleHandle);
//  GetWorld()->GetTimerManager().ClearTimer(L_CompletedPuzzleHandle);
//
//  LeftAssetAudioComponent->Stop();
//  RightAssetAudioComponent->Stop();
//
//  OnLockpickFailure.Broadcast();
//}
//
//void ULockpickWidget::L_StartedPuzzle()
//{
//  if (bIsInMinigame) 
//  {
//    FTimerDelegate TimerDelegate;
//    GetWorld()->GetTimerManager().ClearTimer(L_StartedPuzzleHandle);
//    GetWorld()->GetTimerManager().ClearTimer(L_CompletedPuzzleHandle);
//    TimerDelegate.BindUFunction(this, FName("L_StartedFuntionPuzzle"));
//    GetWorld()->GetTimerManager().SetTimer(L_StartedPuzzleHandle, TimerDelegate, L_TimeStartedStep, true);
//
//    LeftAssetAudioComponent->Play();
//  }
//}
//
//void ULockpickWidget::L_CompletedPuzzle()
//{
//  FTimerDelegate TimerDelegate;
//  GetWorld()->GetTimerManager().ClearTimer(L_CompletedPuzzleHandle);
//  GetWorld()->GetTimerManager().ClearTimer(L_StartedPuzzleHandle);
//  TimerDelegate.BindUFunction(this, FName("L_CompletedFuntionPuzzle"));
//  GetWorld()->GetTimerManager().SetTimer(L_CompletedPuzzleHandle, TimerDelegate, L_TimeCompletedStep, true);
//
//  LeftAssetAudioComponent->Stop();
//}
//
//void ULockpickWidget::R_StartedPuzzle()
//{
//  if (bIsInMinigame)
//  {
//    FTimerDelegate TimerDelegate;
//    GetWorld()->GetTimerManager().ClearTimer(R_StartedPuzzleHandle);
//    GetWorld()->GetTimerManager().ClearTimer(R_CompletedPuzzleHandle);
//    TimerDelegate.BindUFunction(this, FName("R_StartedFuntionPuzzle"));
//    GetWorld()->GetTimerManager().SetTimer(R_StartedPuzzleHandle, TimerDelegate, R_TimeStartedStep, true);
//
//    RightAssetAudioComponent->Play();
//  }
//}
//
//void ULockpickWidget::R_CompletedPuzzle()
//{
//  FTimerDelegate TimerDelegate;
//  GetWorld()->GetTimerManager().ClearTimer(R_CompletedPuzzleHandle);
//  GetWorld()->GetTimerManager().ClearTimer(R_StartedPuzzleHandle);
//  TimerDelegate.BindUFunction(this, FName("R_CompletedFuntionPuzzle"));
//  GetWorld()->GetTimerManager().SetTimer(R_CompletedPuzzleHandle, TimerDelegate, R_TimeCompletedStep, true);
//
//  RightAssetAudioComponent->Stop();
//}
//
//void ULockpickWidget::PuzzleFailExit() 
//{
//  if (L_PuzzleActualProgress <= 5.f || R_PuzzleActualProgress <= 5.f) 
//  {
//    L_PuzzleActualProgress = 0.f;
//    R_PuzzleActualProgress = 0.f;
//  }
//
//  LockPickFailure();
//}
//
//void ULockpickWidget::L_StartedFuntionPuzzle()
//{
//  L_PuzzleActualProgress = UpdateBar(true, L_PuzzleActualProgress,L_BasePuzzleSlider);
//}
//
//void ULockpickWidget::L_CompletedFuntionPuzzle()
//{
//  L_PuzzleActualProgress = UpdateBar(false, L_PuzzleActualProgress,L_BasePuzzleSlider);
//}
//
//void ULockpickWidget::R_StartedFuntionPuzzle()
//{
//  R_PuzzleActualProgress = UpdateBar(true, R_PuzzleActualProgress,R_BasePuzzleSlider);
//}
//
//void ULockpickWidget::R_CompletedFuntionPuzzle()
//{
//  R_PuzzleActualProgress = UpdateBar(false, R_PuzzleActualProgress,R_BasePuzzleSlider);
//}
//
//float ULockpickWidget::UpdateBar(bool _incremental, float _progress,URadialSlider* _Slider_)
//{
//  if (_incremental)
//  {
//    ++_progress;
//  }
//
//  else
//  {
//    --_progress;
//  }
//
//  if (_progress <= 0.f || _progress >= 100.f)
//  {
//    _progress = 0.f;
//  }
//
//  _Slider_->SetValue(_progress * 0.01);
//  return _progress;
//}
