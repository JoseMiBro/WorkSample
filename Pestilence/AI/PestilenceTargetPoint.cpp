#include "PestilenceTargetPoint.h"

APestilenceTargetPoint::APestilenceTargetPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	Pivot->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Pivot->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	Pivot->SetupAttachment(RootComponent);
}

void APestilenceTargetPoint::SetPointActive(bool _bPointActive)
{
	bPointActive = _bPointActive;
}

bool APestilenceTargetPoint::GetPointActive() const
{
	return bPointActive;
}