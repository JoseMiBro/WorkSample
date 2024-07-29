// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RumourZone.generated.h"

class UBoxComponent;
class UConversationAsset;
class APestilenceCharacter;
class UAudioComponent;
class ARumourZone;
class UPestilenceGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConversationStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConversationEnd);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisable, ARumourZone*, Rumour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRange, bool, IsActivated, ARumourZone*, Rumour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnConversationText, const FText&, ConversationText, bool, IsImportantLine, APestilenceCharacter*, TalkCharacter, bool, IsDialogueImportant, ARumourZone*, Rumour);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImportantText, int, ImportantText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetOwner, ARumourZone*, Rumour);

UCLASS()
class PESTILENCE_API ARumourZone : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FOnConversationStart OnConversationStart;

	UPROPERTY()
		FOnConversationEnd OnConversationEnd;

	UPROPERTY()
		FOnImportantText OnImportantText;

		UPROPERTY()
		FOnDisable OnDisable;

		UPROPERTY()
		FOnRange OnRange;

		UPROPERTY()
		FOnConversationText OnConversationText;

		UPROPERTY()
		FOnSetOwner OnSetOwner;

	UPROPERTY(BlueprintReadOnly, Category = "Conversation")
		TArray<TObjectPtr<APestilenceCharacter>> TalkCharactersInTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TArray<TObjectPtr<APestilenceCharacter>> TalkCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TObjectPtr<UBoxComponent> EnterTriggerVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TObjectPtr<UBoxComponent> ExitTriggerVolume;

	UPROPERTY()
		bool bIsConversationStarted = false;

	UPROPERTY()
		bool bIsPlayerInZone = false;

	UPROPERTY()
		bool bIsTriggerImportant = false;

		UPROPERTY()
		bool bIsInDistance = false;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TObjectPtr<UConversationAsset> ConversationBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TObjectPtr<UConversationAsset> ArchetypeIncorrectConversation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TObjectPtr<UConversationAsset> AlternativeConversation;

	UPROPERTY(BlueprintReadOnly, Category = "Conversation")
		TObjectPtr<UConversationAsset> LastConversation;

	int LineCount = 0;
	float Timer = 0;

	bool bIsFirstLine = true;
	bool bCanTalk = true;
	bool bShouldShowConversation = false;

	TObjectPtr<UPestilenceGameInstance> PestilenceGameInstance;

public:	
	// Sets default values for this actor's properties
	ARumourZone();

protected:
	// Called when the game starts or when spawned
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  UFUNCTION()
    void OnWidgetsCreated();

	TObjectPtr<UAudioComponent> Audio;

	UFUNCTION(BlueprintNativeEvent, Category = "Conversation")
		void CheckBeginPlay();

		UFUNCTION()
		void IsOvelapp();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void TimerCheck(float DeltaTime);

	void InitConversation(UConversationAsset* _ConversationAsset);

	void PlayAnimation(UConversationAsset* _ConversationAsset);

	void StopAnimation(UConversationAsset* _ConversationAsset);

	UFUNCTION(BlueprintCallable, Category = "Conversation")
		bool ArchetypeAndGenderCheck(UConversationAsset* _ConversationAsset, UConversationAsset* _ArchetypeIncorrectConversation);

	UFUNCTION()
		bool IsSameConversation(UConversationAsset* _ConversationAsset);

	UFUNCTION()
		bool LineCountCheck(UConversationAsset* _ConversationAsset);

	UFUNCTION(BlueprintCallable, Category = "Conversation")
		bool NotLoopAndChangeCheck(UConversationAsset* _ConversationAsset, UConversationAsset* _AlternativeAsset);

	UFUNCTION()
		bool ConversationCharactersCheck(UConversationAsset* _ConversationAsset);

		UFUNCTION()
		bool BaseConversationCharactersCheck(UConversationAsset* _ConversationAsset);

	UFUNCTION()
		bool BaseCheck(UConversationAsset* _ConversationAsset);

	UFUNCTION(BlueprintNativeEvent, Category = "Conversation")
		bool SpecialCheck(UConversationAsset* _ConversationAsset);

	UFUNCTION(BlueprintImplementableEvent, Category = "Conversation")
		bool ChangeImportantZone(bool bIsImportant);

	UFUNCTION()
		bool IsImportantCheck(UConversationAsset* _ConversationAsset, int _LineCount);

	bool ObjectiveCheck(UConversationAsset* _ConversationAsset);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Conversation")
		void UpdateObjective(int Index);

	UFUNCTION()
		bool IsAllImportantCheck();

	UFUNCTION(BlueprintCallable, Category = "Conversation")
		void ConfirmationConversation(UConversationAsset* _ConversationAsset);

	UFUNCTION()
		virtual void Conversation(UConversationAsset* _ConversationAsset);

	UFUNCTION()
		void OnActorEnterCloseTrigger(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
											bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SetActorEnter(class AActor* OtherActor);

	UFUNCTION()
  void OnActorExitCloseTrigger(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnActorExitFarTrigger(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void QuickLoadReset();

		UFUNCTION(BlueprintCallable, Category = "Conversation")
		void ActivateRumourDistance();

		UFUNCTION(BlueprintCallable, Category = "Conversation")
		void DisableRumourDistance();

		UFUNCTION()
		void SetCanTalkTrue();
		UFUNCTION()
		void SetCanTalkFalse();
};
