// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Characters/PestilenceDialogueCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "Pestilence/UI/PestilenceHubManager.h"
#include "Pestilence/UI/PestilenceDialogueBoxUserWidget.h"

APestilenceDialogueCharacter::APestilenceDialogueCharacter()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));

	CapsuleComp->InitCapsuleSize(55.f, 96.0f);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));

	Mesh->SetupAttachment(CapsuleComp);
	Mesh->bCastDynamicShadow = false;
	Mesh->CastShadow = false;
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
}

void APestilenceDialogueCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		TArray<AActor*> NPCs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APestilenceHubManager::StaticClass(), NPCs);

		for (AActor* Actor : NPCs)
		{
			m_HubManager = Cast<APestilenceHubManager>(Actor);
		}
	}
}

void APestilenceDialogueCharacter::DefaultInteraction(AActor* Actor)
{
	Super::DefaultInteraction(Actor);

	if (Dialogues.Num() <= m_dialogueIndex)
	{
		m_dialogueIndex = 0;
		//m_HubManager->OutViewport(m_HubManager->DialogueWidgetInstance);
		return;
	}

	if (m_HubManager->DialogueWidgetInstance)
	{
		//m_HubManager->InViewport(m_HubManager->DialogueWidgetInstance);
		OnDialogueDelegate.Broadcast(SpeakerExternalName, Dialogues[m_dialogueIndex]);
		++m_dialogueIndex;
	}
}
