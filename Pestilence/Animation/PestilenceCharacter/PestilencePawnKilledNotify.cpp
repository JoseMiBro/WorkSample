// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilencePawnKilledNotify.h"
#include "../../Characters/PestilenceCharacter.h"
#include "../../AI/PestilenceAIController.h"
#include "../../AI/Behaviors/blackboard_keys.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "../../Components/FailState/FailStateComponent.h"
#include "Kismet/GameplayStatics.h"

void UPestilencePawnKilledNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
  Super::Notify(MeshComp, Animation, EventReference);

  if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) return;

  APestilenceCharacter* Character = Cast<APestilenceCharacter>(MeshComp->GetOwner());

  if (!IsValid(Character)) return;

  IPestilenceAIInterface* PestilenceAIInterface = Cast<IPestilenceAIInterface>(Character);
  EPestilenceTypeCharacter Type = PestilenceAIInterface->GetTypeCharacter();

  if (Type != EPestilenceTypeCharacter::Guard) return;

  APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(MeshComp, 0);
  APestilenceCharacter* PlayerCharacter = Cast<APestilenceCharacter>(PlayerPawn);
  if(PlayerCharacter && PlayerCharacter->GetFailStateComponent()->GetKillerActor() == MeshComp->GetOwner())
      PlayerCharacter->GetFailStateComponent()->StartFallToDeath();
  else
  {
    APestilenceAIController* PestilenceAIController = Character->GetControllerAI();

    if (!IsValid(PestilenceAIController)) return;

    UBlackboardComponent* Blackboard = PestilenceAIController->GetBlackboard();
    if (!IsValid(Blackboard)) return;
    Blackboard->SetValueAsBool(Keys::AnimKillNotify, true);
  }
}
