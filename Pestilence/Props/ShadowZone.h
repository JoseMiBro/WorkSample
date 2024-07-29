// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShadowZone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetPlayerInShadow,bool,IsInShadow);

UCLASS()
class PESTILENCE_API AShadowZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShadowZone();

	UPROPERTY()
		FSetPlayerInShadow OnPlayerInShadow;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void IsPlayerInShadow(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
		bool PlayerSpawnedInShadow() const;
};
