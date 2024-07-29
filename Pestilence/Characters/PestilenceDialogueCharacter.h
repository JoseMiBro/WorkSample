// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pestilence/InteractiveActors/InteractiveActor.h"
#include "PestilenceDialogueCharacter.generated.h"

class UCapsuleComponent;
class APestilenceHubManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueDelegate, FString, _SpeakerName, FString, _Dialogue);

/**
 * 
 */
UCLASS()
class PESTILENCE_API APestilenceDialogueCharacter : public AInteractiveActor
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FDialogueDelegate OnDialogueDelegate;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
		TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
		UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
		FString SpeakerExternalName;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
		TArray<FString> Dialogues;

	UPROPERTY()
		int m_dialogueIndex = 0;

	UPROPERTY()
		APestilenceHubManager* m_HubManager;

public:
	APestilenceDialogueCharacter();
protected:
	virtual void BeginPlay() override;

public:
	TObjectPtr<USkeletalMeshComponent> GetMesh() const { return Mesh; }
	virtual void DefaultInteraction(AActor* Actor) override;

	UFUNCTION()
		FString GetExtenalName() { return SpeakerExternalName; }

	UFUNCTION()
		const TArray<FString> GetDialogue() const { return Dialogues; }

	UFUNCTION()
		int GetDialogueIndex() { return m_dialogueIndex; }
};
