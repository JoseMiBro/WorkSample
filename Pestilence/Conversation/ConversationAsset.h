// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Pestilence/Enums/Archetype.h"
#include "Pestilence/Enums/Gender.h"
#include "ConversationAsset.generated.h"

class APestilenceCharacter;
class USoundBase;
class UAnimSequence;

USTRUCT(BlueprintType)
struct PESTILENCE_API FConversationLine
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		int IDTalkCharacter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		USoundBase* SoundLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		float TextOffsetDuration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TObjectPtr<UAnimSequence> TalkAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		bool IsLineImportant = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		int IDDiaryText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		EGender RestrictedGenders = EGender::Other;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		bool ChangesObjective = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		int ObjectiveIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		bool LaunchesFunct = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		FString FunctionName;

};

UENUM(BlueprintType)
enum class EConversationType : uint8
{
	Base							UMETA(DisplayName = "Base"),
	InCorrectToAccess	UMETA(DisplayName = "InCorrectToAccess"),
	Default						UMETA(DisplayName = "Default")
};

UCLASS(BlueprintType)
class PESTILENCE_API UConversationAsset : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TArray<FConversationLine> Lines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TArray<EArchetype> RestrictedArchetypes;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	//	EConversationType ConversationType = EConversationType::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		bool CanLoop = false;

	UPROPERTY(BlueprintReadOnly, Category = "Conversation")
		bool IsDialogueImportant = false;

	UPROPERTY(BlueprintReadOnly, Category = "Conversation")
		int DialogueImportantCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Conversation")
		TArray<TObjectPtr<APestilenceCharacter>> TalkCharacters;

	UPROPERTY(BlueprintReadOnly, Category = "Conversation")
		bool bOnFinishConversation = false;

public:
	void ReverseLines();

	bool IsConversationImportant();

	//void AllTalkCharacters();
};
