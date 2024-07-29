// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PestilenceAIController.h"
#include "PestilenceAIDoormanController.generated.h"

class APestilenceTargetPoint;
class ARumourZone;
class ADoormanRumourZone;
class AArchetypeZone;
/**
 * 
 */
UCLASS()
class PESTILENCE_API APestilenceAIDoormanController : public APestilenceAIController
{
	GENERATED_BODY()

	public:
		APestilenceAIDoormanController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
		UPROPERTY(EditAnywhere,Category="Pestilence Perception")
			TObjectPtr<AArchetypeZone> AssignedZone;
		UPROPERTY(EditAnywhere,Category="Pestilence Sight")
			TObjectPtr<APestilenceTargetPoint> WarningTargetPoint;
		float GetWarningDistance(){ return WarningDistance; };
	private:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaSeconds) override;
		virtual void OnPossess(APawn* InPawn) override;
		
		UPROPERTY(EditAnywhere,Category="Pestilence Perception")
			TObjectPtr<ADoormanRumourZone> RumourZone;
		
		UFUNCTION()
			void OnPlayerEnter(AActor* OverlappedActor, AActor* OtherActor);
		UFUNCTION()
			void OnPlayerExit(AActor* OverlappedActor, AActor* OtherActor);
		UFUNCTION()
			void ControlPossesions();
		
		UPROPERTY(EditAnywhere,Category="Pestilence Sight")
			float WarningDistance;

		bool bHasWarned = false;
		bool bCorrectConversationPlayed = false;
		bool bIsInMyZone = false;
};
