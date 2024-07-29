// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowZone.h"

#include "Kismet/GameplayStatics.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/Components/Perception/PestilenceSightComponent.h"

// Sets default values
AShadowZone::AShadowZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AShadowZone::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this,&AShadowZone::IsPlayerInShadow);
	OnActorEndOverlap.AddDynamic(this,&AShadowZone::IsPlayerInShadow);

	if(PlayerSpawnedInShadow())
	{
		IsPlayerInShadow(
			this,
			UGameplayStatics::GetPlayerController(GetWorld(),0)->GetPawn());
	}
}

void AShadowZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnActorBeginOverlap.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void AShadowZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShadowZone::IsPlayerInShadow(AActor* OverlappedActor, AActor* OtherActor)
{
	if(UGameplayStatics::GetPlayerController(GetWorld(), 0) == nullptr) return;
	
	const TObjectPtr<APawn> PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	if(OtherActor == PlayerPawn)
	{
		OnPlayerInShadow.Broadcast(PlayerSpawnedInShadow());

		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(this, APestilenceAIController::StaticClass(),OutActors);

		TObjectPtr<APestilenceAIController> AIController;
		for(int i = 0; i < OutActors.Num();i++)
		{
			AIController = Cast<APestilenceAIController>(OutActors[i]);
			if(AIController)
			{
				TObjectPtr<UPestilenceSightComponent> Component = AIController->FindComponentByClass<UPestilenceSightComponent>();
				if (Component) 
				{
					Component->SetOnShadow(PlayerSpawnedInShadow());
				}
			}
		}
	}
}

bool AShadowZone::PlayerSpawnedInShadow() const
{
	return IsOverlappingActor(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
}


