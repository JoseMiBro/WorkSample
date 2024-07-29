#include "TakeDownComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/Interaction/InteractorComponent.h"
#include "Pestilence/Components/Interaction/InteractableComponent.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Components/Perception/SuspiciousActionSenderComponent.h"
#include "Pestilence/Components/Interaction/InteractorComponent.h"
#include "../Abilities/PestilenceAbilityComponent.h"

// ------------------------------

UTakeDownComponent::UTakeDownComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// ------------------------------

void UTakeDownComponent::BeginPlay()
{
    Super::BeginPlay();

    if (auto* InteractableBase = GetOwner()->GetComponentByClass(UInteractorComponent::StaticClass()))
    {
        auto* Interactor = Cast<UInteractorComponent>(InteractableBase);
        Interactor->OnTriedToInteract.AddDynamic(this, &UTakeDownComponent::OnTriedToInteractCallback);
    }

    if (ensure(GetOwner()->IsA(APestilenceCharacter::StaticClass())))
    {
        const auto* Character = Cast<APestilenceCharacter>(GetOwner());
        MovementComponent = Character->GetPestilenceMovementComponent();
    }
}

// ------------------------------

void UTakeDownComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if (!ExecutingTakeDown)
        return;

    SelfStunTime += DeltaTime;

    if (SelfStunTime >= TakeDownTime)
    {
        SelfStunTime = 0.f;
        ExecutingTakeDown = false;
        DisableSelfStun();
    }
}

// ------------------------------

bool UTakeDownComponent::IsBehindInteractableActor(UInteractableComponent* Interactable) const
{
    const FVector OtherForward = Interactable->GetOwner()->GetActorForwardVector();
    const FVector Forward = GetOwner()->GetActorForwardVector();
    return Forward.Dot(OtherForward) > 0;
}

// ------------------------------

void UTakeDownComponent::EnableSelfStun()
{
    MovementComponent->EnableMovement(false);
    MovementComponent->EnableLook(false);
    bWasCrouched = MovementComponent->bWantsToCrouch;
    if (bWasCrouched) MovementComponent->StartCrouchInterp(false);
    OnTakeDownStarted.Broadcast();
    const APestilenceCharacter* Character = Cast<APestilenceCharacter>(GetOwner());
    if (!Character) return;

    Character->GetInteractorComponent()->Enable(false);
    Character->GetPestilenceAbilityComponent()->EnableAbilities(false);

}

// ------------------------------

void UTakeDownComponent::DisableSelfStun()
{
    MovementComponent->EnableMovement(true);
    MovementComponent->EnableLook(true);
    if (bWasCrouched) MovementComponent->StartCrouchInterp(true);
    OnTakeDownFinished.Broadcast();
    APestilenceCharacter* Character = Cast<APestilenceCharacter>(GetOwner());
    if (!Character) return;

    Character->GetInteractorComponent()->Enable(true);
    Character->GetPestilenceAbilityComponent()->EnableAbilities(true);
    Character->StopTakeDownParticles();
}

// ------------------------------

void UTakeDownComponent::OnTriedToInteractCallback(bool bPerformed, bool bSuccess, UInteractableComponent* InteractableActor)
{
    if (!bPerformed || !bSuccess || !InteractableActor ||!InteractableActor->GetOwner()->IsA(APestilenceCharacter::StaticClass()))
        return;

    if (!IsBehindInteractableActor(InteractableActor))
        return;

    auto* Other = Cast<APestilenceCharacter>(InteractableActor->GetOwner());
    if (!Other->CanBeTakeDown()) return;

    APestilenceAIController* AIController = Other->GetControllerAI();

    //IPestilenceAIInterface* Interface = Cast<IPestilenceAIInterface>(Other);
    //EPestilenceTypeCharacter TypeOfCharacter = Interface->GetTypeCharacter();
    //switch (TypeOfCharacter)
    //{
    //case EPestilenceTypeCharacter::Pawn:
    //{
    //    if (AIController->GetCurrentState() != EPestilenceStates::Infected)
    //        AIController->SetCurrentState(EPestilenceStates::Infected);
    //} break;
    //default:
    //{
    EPestilenceStates LastState = TEnumAsByte<EPestilenceStates>(AIController->GetBlackboard()->GetValueAsEnum(Keys::State));
    if (AIController->GetCurrentState() != EPestilenceStates::Stun && LastState != EPestilenceStates::Stun && AIController->GetCurrentState() != EPestilenceStates::Infected)
    {
        //if(LastState != EPestilenceStates::Stun)
        //{
        //    if(!AIController->ChangeOfStateReported)
        //    {
        //        AIController->ChangeOfStateReported = true;
        //        AIController->OnChangeState.Broadcast(EPestilenceStates::Stun);
        //    }
        //}
        AIController->GetBlackboard()->SetValueAsEnum(Keys::LastState, GetTypeHash(EPestilenceStates::Stun));
        AIController->SetCurrentState(EPestilenceStates::Stun);
        AIController->OnChangeState.Broadcast(EPestilenceStates::Stun);
    }else if (AIController->GetCurrentState() == EPestilenceStates::Stun)
    {
        AIController->GetBrainComponent()->RestartLogic();
        AIController->OnStunReaplied.Broadcast();
    }
    //} break;
    //}

    /*TakeDownTime = AIController->BTPriorityConfig->FindRow<FBTPriorityConfigRow>
        (AIController->StateToString(EPestilenceStates::Stun), "")->MinStateTime;*/
    
    ExecutingTakeDown = true;
    EnableSelfStun();

    if (GetOwner()->IsA(APestilenceCharacter::StaticClass()))
    {
        APestilenceCharacter* Owner = Cast<APestilenceCharacter>(GetOwner());
        Owner->GetSuspiciousActionSenderComponent()->SubmitSuspiciousAction(GetOwner(), ESuspiciousActionType::TakeDown);
        Owner->StartTakeDownParticles();
    }
    FVector LookAt = (Other->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
    LookAt.Z = 0;
    FVector TakeDownPos = Other->GetActorLocation() + -LookAt * TakeDownDistance;
    
    MovementComponent->StartTakeDown(TakeDownPos);
    MovementComponent->StartLookAt(Other->GetActorLocation(), LookAtTime);
    Other->GetPestilenceMovementComponent()->StartLookAt(Other->GetActorLocation() + GetOwner()->GetActorForwardVector(), LookAtTime);
}

// ------------------------------

void UTakeDownComponent::OnLookingInteractableCallback(UInteractableComponent* /*Interactable*/)
{
    /*if (!Interactable->GetOwner()->IsA(APestilenceCharacter::StaticClass()) || !IsBehindInteractableActor(InteractableActor))
        return;*/
}

// ------------------------------