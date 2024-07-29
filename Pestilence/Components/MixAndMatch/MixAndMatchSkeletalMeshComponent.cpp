// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Components/MixAndMatch/MixAndMatchSkeletalMeshComponent.h"

#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/MixAndMatch/MixAndMatchComponent.h"


#if WITH_EDITOR
void UMixAndMatchSkeletalMeshComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	if (!IsValid(GetOwner())) return; 
	const TObjectPtr<APestilenceCharacter> pCharacter = Cast<APestilenceCharacter>(GetOwner());
	if (!IsValid(pCharacter)) return;
	
	TObjectPtr<UMixAndMatchComponent> pMix = pCharacter->GetMixAndMatchComponent();
	//if (IsValid(pMix))
		//pMix->SaveMeshesAndMaterial();
}
#endif