// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PestilenceDiaryUserWidget.generated.h"

class UVerticalBox;
class URichTextBlock;
class UButton;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnImportantTextFeedback);

USTRUCT(BlueprintType)
struct PESTILENCE_API FDiaryEntrance
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diary")
		int IDDiary = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diary")
		FText DiaryEntrance;
};

UCLASS()
class PESTILENCE_API UPestilenceDiaryUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diary")
		TArray<FDiaryEntrance> DiaryEntrance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diary")
		int MaxTextInPage = 10;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Diary")
		URichTextBlock* DiaryEntranceTemplate;

	UPROPERTY(BlueprintReadOnly, Category = "Diary")
		TArray<int> DiaryActiveEntrance;

	UPROPERTY(BlueprintReadOnly, Category = "Diary")
		TArray<TObjectPtr<URichTextBlock>> DiaryTextBlockEntrance;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Diary")
		UVerticalBox* L_VerticalBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Diary")
		UVerticalBox* R_VerticalBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Diary")
		UButton* NextPage;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Diary")
		UButton* BeforePage;

	UPROPERTY(BlueprintReadOnly, Category = "Diary")
		int PageCount = 1;

	UPROPERTY()
		FOnImportantTextFeedback OnImportantTextFeedback;
	
  UPROPERTY(EditAnywhere, Category = "Diary")
  TObjectPtr<USoundBase> OpenDiarySound;
  UPROPERTY(EditAnywhere, Category = "Diary")
  TObjectPtr<USoundBase> CloseDiarySound;
private:

  UPROPERTY(EditAnywhere, Category = "Diary")
  TObjectPtr<USoundBase> NextPageSound;

//protected:
//	virtual void NativeConstruct() override;

public:
	virtual bool Initialize() override;

public:
	//UFUNCTION(BlueprintImplementableEvent)
	//	void OnImportantTextModified(const FText& ImportantTextDiary);
	UFUNCTION()
		void ActiveDiary(int _ID);

	UFUNCTION()
		void SetActiveDiary(int _ID);
	//
	UFUNCTION()
		void ActiveDiaryMassive(TArray<int> IDs);

	UFUNCTION()
		void ChangePageUP();

	UFUNCTION()
		void ChangePageDown();

	UFUNCTION()
		void QuickLoadReset();
};
