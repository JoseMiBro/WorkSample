// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCManager.generated.h"

class APestilenceGameMode;
class APestilenceCharacter;

UCLASS()
class PESTILENCE_API ANPCManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddNPC(APestilenceCharacter* CharToAdd);
	void RemoveNPC(APestilenceCharacter* CharToRemove);
	
	const TArray<TObjectPtr<APestilenceCharacter>>& GetNpcs() const;

	UFUNCTION()
		void EnableNPC(APawn* NPC, AController* NPC_Controller,bool Enable, int AnimTick) const;
	UFUNCTION()
		void EnableController(APestilenceAIController* NPC_Controller, float Tick,bool Enable) const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere,Category="NPCs Optimization")
		int MaxNPCsOnLevel = 300;
	UPROPERTY(EditAnywhere,Category="NPCs Optimization")
		TArray<TObjectPtr<APestilenceCharacter>> NPCsOnLevel;
	
	UPROPERTY(EditAnywhere, Category="NPCs Optimization")
		float UpdateTime = 0.f;
	
	UPROPERTY(EditAnywhere, Category="NPCs Optimization")
		float UpdateTimeReset = 2.f;
	
	UPROPERTY(EditAnywhere, Category="NPCs Optimization")
		float ActiveNPCsRadius = 3500.f;
	UPROPERTY(EditAnywhere, Category="NPCs Optimization")
		float FunctionalNPCsRadius = 1500.f;

	UPROPERTY(EditAnywhere, Category="NPCs Optimization")
		float MinTickTime = 0.0f;
	UPROPERTY(EditAnywhere, Category="NPCs Optimization")
		float MaxTickTime = 0.5f;
	
	UFUNCTION()
		void OptimizeNPCs();
	UFUNCTION()
		void EnableComponent(UActorComponent* Component, float Tick, bool Enable) const;
};
