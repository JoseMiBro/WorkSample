// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceStepNotify.h"
#include "Kismet/GameplayStatics.h"
#include "../../Characters/PestilenceCharacter.h"
#include "../../Components/Movement/PestilenceMovementComponent.h"

void UPestilenceStepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
  Super::Notify(MeshComp, Animation, EventReference);

  if (!MeshComp || !MeshComp->GetOwner()) return;
  
  APestilenceCharacter* Character = Cast<APestilenceCharacter>(MeshComp->GetOwner());

  if (!Character || !Character->GetPestilenceMovementComponent() || Character->GetPestilenceMovementComponent()->IsFalling()) return;

  FVector Location = MeshComp->GetSocketLocation(SocketName);
  UPestilenceMovementComponent* MovementComponent = Character->GetPestilenceMovementComponent();
  if (MovementComponent->IsCrouching() || MovementComponent->Velocity.SizeSquared() < MovementComponent->GetMediumSpeed() * MovementComponent->GetMediumSpeed())
    UGameplayStatics::PlaySoundAtLocation(MeshComp, Character->GetPestilenceMovementComponent()->CrouchStepSound, Location, VolumeMultiplier, PitchMultiplier, StartTime);
  else if (MovementComponent->Velocity.SizeSquared() < MovementComponent->GetFastSpeed() * MovementComponent->GetFastSpeed())
    UGameplayStatics::PlaySoundAtLocation(MeshComp, Character->GetPestilenceMovementComponent()->WalkStepSound, Location, VolumeMultiplier, PitchMultiplier, StartTime);
  else
    UGameplayStatics::PlaySoundAtLocation(MeshComp, Character->GetPestilenceMovementComponent()->RunStepSound, Location, VolumeMultiplier, PitchMultiplier, StartTime);
}
