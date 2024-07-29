// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Animation/PestilenceCharacter/PestilenceUsingDoorNotify.h"

#include "PestilenceCharacterAnimInstance.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"

void UPestilenceUsingDoorNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) return;

	APestilenceCharacter* Character = Cast<APestilenceCharacter>(MeshComp->GetOwner());

	if (!IsValid(Character))
		return;
	
	UPestilenceMovementComponent* MovementComponent = Character->GetPestilenceMovementComponent();
	
	if (MovementComponent)
		MovementComponent->StartUsingDoor(false, 0.f, 0.f);
}
