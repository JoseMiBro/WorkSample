#include "PlayerHUDWidget.h"

#include "Components/ProgressBar.h"

void UPlayerHUDWidget::SetIntegrityBar(const float CurrentAmount, const float MaxAmount)
{
	if (!IntegrityBar) return;
	IntegrityBar->SetPercent(CurrentAmount / MaxAmount);
}