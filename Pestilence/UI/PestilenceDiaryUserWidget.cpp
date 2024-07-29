// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceDiaryUserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

#include "Pestilence/Props/RumourZone.h"
#include "Pestilence/Props/DiaryZone.h"
#include "Pestilence/Core/PestilenceSaveGameSubsystem.h"

bool UPestilenceDiaryUserWidget::Initialize()
{
	bool parentSuccess = Super::Initialize();
	if (!parentSuccess)
	{
		return parentSuccess;
	}

	if (GetWorld())
	{
		TArray<AActor*> RumoursZones;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARumourZone::StaticClass(), RumoursZones);

		for (AActor* Actor : RumoursZones)
		{
			ARumourZone* pRumourZone = Cast<ARumourZone>(Actor);
			if (pRumourZone) 
			{
				pRumourZone->OnImportantText.AddDynamic(this, &UPestilenceDiaryUserWidget::ActiveDiary);
			}
		}

		TArray<AActor*> DiaryZones;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADiaryZone::StaticClass(), DiaryZones);

		for (AActor* Actor : DiaryZones)
		{
			ADiaryZone* pDiaryZone = Cast<ADiaryZone>(Actor);
			if (pDiaryZone)
			{
				pDiaryZone->OnDiaryText.AddDynamic(this, &UPestilenceDiaryUserWidget::ActiveDiary);
			}
		}
	}

	UGameInstance* GameInstance = GetGameInstance();

	if (GameInstance) 
	{
		UPestilenceSaveGameSubsystem* GameSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
		if (GameSubsystem) 
		{
			GameSubsystem->OnLoad.AddDynamic(this, &UPestilenceDiaryUserWidget::QuickLoadReset);
		}
	}

	return true;
}

void UPestilenceDiaryUserWidget::ActiveDiary(int _ID)
{
	if (DiaryActiveEntrance.Contains(_ID) || !DiaryEntrance.IsValidIndex(_ID))
	{
		return;
	}

	SetActiveDiary(_ID);
	OnImportantTextFeedback.Broadcast();
}

void UPestilenceDiaryUserWidget::SetActiveDiary(int _ID)
{
	DiaryActiveEntrance.Add(_ID);

	if (DiaryActiveEntrance.Num() <= MaxTextInPage * 2)
	{
		URichTextBlock* auxRichText = NewObject<URichTextBlock>(this);

		DiaryTextBlockEntrance.Add(auxRichText);

		if (DiaryActiveEntrance.Num() <= MaxTextInPage)
		{
			L_VerticalBox->AddChild(auxRichText);
		}

		else if (MaxTextInPage < DiaryActiveEntrance.Num() && DiaryActiveEntrance.Num() <= MaxTextInPage * 2)
		{
			R_VerticalBox->AddChild(auxRichText);
		}
		auxRichText->SetTextStyleSet(DiaryEntranceTemplate->GetTextStyleSet());
		auxRichText->SetAutoWrapText(DiaryEntranceTemplate->GetAutoWrapText());
		auxRichText->SetText(DiaryEntrance[_ID].DiaryEntrance);
	}
	else 
	{
		if (PageCount != 1 && DiaryActiveEntrance.Num() < MaxTextInPage * 2 * PageCount - 1)
		{
			DiaryTextBlockEntrance[DiaryActiveEntrance.Num() - (DiaryTextBlockEntrance.Num() * (PageCount - 1) + 1)]->SetText(DiaryEntrance[_ID].DiaryEntrance);
		}
	}
}

void UPestilenceDiaryUserWidget::ActiveDiaryMassive(TArray<int> IDs)
{
	for (int i = 0; i < IDs.Num(); ++i) 
	{
		if (DiaryActiveEntrance.Contains(IDs[i]) || !DiaryEntrance.IsValidIndex(IDs[i]))
		{
			return;
		}
		SetActiveDiary(IDs[i]);
	}
}

void UPestilenceDiaryUserWidget::ChangePageUP()
{
	if (DiaryActiveEntrance.Num() > MaxTextInPage * 2 * PageCount)
	{
		int auxCount = 0;

		for (int i = MaxTextInPage * 2 * PageCount; i < MaxTextInPage * 2 * (PageCount + 1); i++)
		{
			if (DiaryActiveEntrance.IsValidIndex(i)) 
			{
				DiaryTextBlockEntrance[auxCount]->SetText(DiaryEntrance[DiaryActiveEntrance[i]].DiaryEntrance);
			}
			else 
			{
				DiaryTextBlockEntrance[auxCount]->SetText(FText::FromString(""));
			}
			++auxCount;
		}
		++PageCount;

		UGameplayStatics::PlaySound2D(this, NextPageSound);
	}
}

void UPestilenceDiaryUserWidget::ChangePageDown()
{
	if (DiaryActiveEntrance.Num() < MaxTextInPage * 2 * PageCount && MaxTextInPage * 2 * (PageCount - 1) > 0)
	{
		int auxCount = DiaryTextBlockEntrance.Num() - 1;
    --PageCount;
    UGameplayStatics::PlaySound2D(this, NextPageSound);
		if (DiaryActiveEntrance.Num() > MaxTextInPage * 2 * PageCount)
		{
			for (int i = DiaryTextBlockEntrance.Num() * PageCount - 1; i >= DiaryTextBlockEntrance.Num() * (PageCount - 1); --i)
			{
				if (DiaryActiveEntrance.IsValidIndex(i * PageCount))
				{
					DiaryTextBlockEntrance[auxCount]->SetText(DiaryEntrance[DiaryActiveEntrance[i * PageCount]].DiaryEntrance);
					--auxCount;
				}
			}
		}
	}
}

void UPestilenceDiaryUserWidget::QuickLoadReset()
{
	L_VerticalBox->ClearChildren();
	R_VerticalBox->ClearChildren();
	DiaryActiveEntrance.Empty();
}
