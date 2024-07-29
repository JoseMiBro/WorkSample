// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceProjectileAppearNotify.h"
#include "../../Components/Abilities/PestilenceAbility.h"
#include "../../Components/Abilities/PestilenceAbilityComponent.h"
#include "../../Characters/PestilenceCharacter.h"

void UPestilenceProjectileAppearNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
  Super::Notify(MeshComp, Animation, EventReference);

  if (!MeshComp || !MeshComp->GetOwner()) return;

  APestilenceCharacter* Character = Cast<APestilenceCharacter>(MeshComp->GetOwner());

  if (!Character || !Character->GetPestilenceAbilityComponent()) return;

  Character->GetPestilenceAbilityComponent()->GetCurrentAbility()->ProjectileAppear();
}
