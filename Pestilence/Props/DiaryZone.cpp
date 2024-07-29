// Fill out your copyright notice in the Description page of Project Settings.

#include "Pestilence/Props/DiaryZone.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Pestilence/Characters/PestilenceCharacter.h"

// Sets default values
ADiaryZone::ADiaryZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create triggers volume
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);

	TriggerVolume->SetBoxExtent(FVector(30, 30, 30), true);

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ADiaryZone::OnActorEnter);
}

// Called when the game starts or when spawned
void ADiaryZone::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADiaryZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADiaryZone::OnActorEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bDoOnce)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			APestilenceCharacter* PestilenceCharacter = Cast<APestilenceCharacter>(OtherActor);
			if (PestilenceCharacter)
			{
				if (PestilenceCharacter->GetController() == PlayerController)
				{
					bDoOnce = true;
					OnDiaryText.Broadcast(IDDiary);
				}
			}
		}
	}
}
