// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiaryZone.generated.h"

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiaryText, int, ImportantText);

UCLASS()
class PESTILENCE_API ADiaryZone : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diary")
		int IDDiary = 0;

	UPROPERTY()
		FOnDiaryText OnDiaryText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
		TObjectPtr<UBoxComponent> TriggerVolume;

private:
	bool bDoOnce = false;

public:
	// Sets default values for this actor's properties
	ADiaryZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnActorEnter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
};
