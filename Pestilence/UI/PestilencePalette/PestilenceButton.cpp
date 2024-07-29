// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilencePalette/PestilenceButton.h"

#include "Components/TextBlock.h"

UPestilenceButton::UPestilenceButton()
{
	OnHovered.AddDynamic(this, &UPestilenceButton::OnHoveredPestilence);
	OnUnhovered.AddDynamic(this, &UPestilenceButton::OnUnhoveredPestilence);
}

void UPestilenceButton::PostLoad()
{
	Super::PostLoad();

}

void UPestilenceButton::OnHoveredPestilence()
{
	if (GetChildAt(0)) 
	{
		UTextBlock* ChildTextBlock = Cast<UTextBlock>(GetChildAt(0));

		if (ChildTextBlock)
		{
			ChildTextBlock->SetRenderOpacity(MaxOpacity);
		}
	}
}

void UPestilenceButton::OnUnhoveredPestilence()
{
	if (GetChildAt(0))
	{
		UTextBlock* ChildTextBlock = Cast<UTextBlock>(GetChildAt(0));

		if (ChildTextBlock)
		{
			ChildTextBlock->SetRenderOpacity(MinOpacity);
		}
	}
}