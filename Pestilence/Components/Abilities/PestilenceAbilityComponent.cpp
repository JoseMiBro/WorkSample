// Fill out your copyright notice in the Description page of Project Settings.

#include "PestilenceAbilityComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h" 	 	
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h" 	 	
#include "PestilenceAbility.h"
#include "States/BaseAbilityState.h"
#include "States/IdleAbilityState.h"
#include "States/PrepareAbilityState.h"
#include "States/CastAbilityState.h"
#include "States/UseAbilityState.h"
#include "../Interaction/InventoryComponent.h"
#include "../Interaction/PickableComponent.h"
#include "Pickable/PickableAbility.h"
#include "../../Core/PestilenceGameMode.h"
#include "States/PrepareNoIntegrityAbilityState.h"


// Sets default values for this component's properties
UPestilenceAbilityComponent::UPestilenceAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPestilenceAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner<APestilenceCharacter>();

  IdleState = NewObject<UIdleAbilityState>(this);
  IdleState->SetAbilityComponent(this);
  PrepareState = NewObject<UPrepareAbilityState>(this);
  PrepareState->SetAbilityComponent(this);
  PrepareNoIntegrityState = NewObject<UPrepareNoIntegrityAbilityState>(this);
  PrepareNoIntegrityState->SetAbilityComponent(this);
  CastState = NewObject<UCastAbilityState>(this);
  CastState->SetAbilityComponent(this);
  UseState = NewObject<UUseAbilityState>(this);
  UseState->SetAbilityComponent(this);

  ChangeState(IdleState);

	CurrentAbilityId = 0;
	
	if (AbilityObjects.Num() > 0)
		EquipAbility(CurrentAbilityId, false);

	Owner->GetInventoryComponent()->OnTakePickable.AddDynamic(this, &UPestilenceAbilityComponent::OnTakePickable);
	Owner->GetInventoryComponent()->OnDropPickable.AddDynamic(this, &UPestilenceAbilityComponent::OnDropPickable);

	if (Owner->GetPestilencePossessionComponent()->IsPossessed())
		Initialize();
	else
    Owner->GetPestilencePossessionComponent()->OnPossessed.AddDynamic(this, &UPestilenceAbilityComponent::Initialize);
}

void UPestilenceAbilityComponent::SelectAbility(int _Ability, bool _bPlaySound /*= true*/)
{
	if (!bCanUseAbilities || _Ability < 0 || MaxAbilities <= _Ability || !CurrentState->CanSelectAbility() || _Ability == AbilityToChange) return;

	AbilityToChange = _Ability;
	OnAbilitySelected.Broadcast(AbilityObjects[_Ability]->GetAbilityType());
  bChangingAbility = true;
  ChangeAbilityTimer = 0.f;
  bPlaySoundOnChangeAbility = _bPlaySound;
}

void UPestilenceAbilityComponent::EquipAbility(int _Ability, bool _bPlaySound /*= true*/)
{
  if (_Ability < 0 || MaxAbilities <= _Ability || !CurrentState->CanSelectAbility()) return;

  bChangingAbility = false;

  if (CurrentState)
  {
    CurrentState->CancelCast();
    CurrentState->CancelPrepare();
  }

  AbilityToChange = _Ability;
  CurrentAbilityId = _Ability;
  CurrentAbility = AbilityObjects[CurrentAbilityId];
  CurrentAbility->Select(_bPlaySound);
  if (bTryPrepare) StartPrepare(); 

  OnAbilityEquipped.Broadcast(CurrentAbility->GetAbilityType());
}

void UPestilenceAbilityComponent::SelectAbilityNext()
{
  int AbilityId = bChangingAbility ? AbilityToChange : CurrentAbilityId;
	SelectAbility((AbilityId + 1) % MaxAbilities);
}

void UPestilenceAbilityComponent::SelectAbilityPrev()
{
  int AbilityId = bChangingAbility ? AbilityToChange : CurrentAbilityId;
	SelectAbility(AbilityId - 1 < 0 ? MaxAbilities - 1 : AbilityId - 1);
}

// Called every frame
void UPestilenceAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
		CurrentState->UpdateState(DeltaTime);

  if(bChangingAbility)
  {
    ChangeAbilityTimer += DeltaTime;
    if(ChangeAbilityTimer > ChangeAbilityTime)
      EquipAbility(AbilityToChange, bPlaySoundOnChangeAbility);
  }
}

void UPestilenceAbilityComponent::ChangeState(UBaseAbilityState* _NewState)
{
	if (CurrentState)
		CurrentState->ExitState();
	CurrentState = _NewState;
	CurrentState->EnterState();

	OnStateChanged.Broadcast(CurrentState->GetStateType());
}

void UPestilenceAbilityComponent::StartCast()
{
  if (!bCanUseAbilities || !CurrentAbility) return;
  if (CurrentState) CurrentState->StartCast();
}

void UPestilenceAbilityComponent::CancelCast()
{
  if (!bCanUseAbilities || !CurrentAbility) return;
  if (CurrentState) CurrentState->CancelCast();
}

void UPestilenceAbilityComponent::StartPrepare() 
{
  bTryPrepare = true;
  if (!bCanUseAbilities || !CurrentAbility) return;
	if (CurrentState) CurrentState->StartPrepare(); 
}

void UPestilenceAbilityComponent::CancelPrepare() 
{ 
  bTryPrepare = false;
  if (!bCanUseAbilities || !CurrentAbility) return;
	if (CurrentState) CurrentState->CancelPrepare();
}

void UPestilenceAbilityComponent::BroadcastNotEnoughIntegrity() const { OnNotEnoughIntegrity.Broadcast(); }

void UPestilenceAbilityComponent::BroadcastCastStarted() const { OnCastStarted.Broadcast(); }
void UPestilenceAbilityComponent::BroadcastCastEnded() const { OnCastEnded.Broadcast(); }

void UPestilenceAbilityComponent::BroadcastPrepareStarted() const { OnPrepareStarted.Broadcast(); }
void UPestilenceAbilityComponent::BroadcastPrepareEnded() const { OnPrepareEnded.Broadcast(); }

void UPestilenceAbilityComponent::BroadcastPrepareNoIntegrityStarted() const { OnPrepareNoIntegrityStarted.Broadcast(); }
void UPestilenceAbilityComponent::BroadcastPrepareNoIntegrityEnded() const { OnPrepareNoIntegrityEnded.Broadcast(); }

void UPestilenceAbilityComponent::BroadcastAbilityStarted() const { OnAbilityStarted.Broadcast(); }
void UPestilenceAbilityComponent::BroadcastAbilityEnded() const { OnAbilityEnded.Broadcast(); }

UPestilenceAbility* UPestilenceAbilityComponent::GetCurrentAbility() const { return CurrentAbility; }

int UPestilenceAbilityComponent::GetCurrentAbilityId() const { return CurrentAbilityId; }
UBaseAbilityState* UPestilenceAbilityComponent::GetCurrentState() const { return CurrentState; }
UBaseAbilityState* UPestilenceAbilityComponent::GetIdleState()  const { return IdleState; }
UBaseAbilityState* UPestilenceAbilityComponent::GetPrepareState() const {	return PrepareState; }
UBaseAbilityState* UPestilenceAbilityComponent::GetPrepareNoIntegrityState() const { return PrepareNoIntegrityState; }
UBaseAbilityState* UPestilenceAbilityComponent::GetCastState() const { return CastState; }
UBaseAbilityState* UPestilenceAbilityComponent::GetUseState() const { return UseState; }

float UPestilenceAbilityComponent::GetCurrentIntegrity() const { return Owner->GetPestilencePossessionComponent()->GetCurrentIntegrity(); }
void UPestilenceAbilityComponent::SetCurrentIntegrity(float _Integrity) { Owner->GetPestilencePossessionComponent()->SetCurrentIntegrity(_Integrity); }

bool UPestilenceAbilityComponent::GetIsTryPrepare() const { return bTryPrepare; }

APestilenceCharacter* UPestilenceAbilityComponent::GetPestilenceOwner() {	return Owner; }

void UPestilenceAbilityComponent::EnableAbilities(bool _bCanUseAbilities)
{
	bCanUseAbilities = _bCanUseAbilities;
  if (!bCanUseAbilities)
  {
    if (CurrentState)
    {
      CurrentState->CancelCast();
      CurrentState->CancelPrepare();
    }
    ChangeState(IdleState);
  }
  else if (bTryPrepare)
    StartPrepare();
}

int UPestilenceAbilityComponent::GetMaxAbilities() const { return MaxAbilities; }

float UPestilenceAbilityComponent::GetPrepareSuspiciousTime() const { return PrepareSuspiciousTime; }

void UPestilenceAbilityComponent::OnTakePickable(UPickableComponent* Pickable, bool bResult)
{
	if (bResult)
  {
    if (CurrentState) CurrentState->CancelPrepare();
		CurrentAbility = PickableAbility;
		CurrentAbility->Select(false);
    OnAbilityEquipped.Broadcast(CurrentAbility->GetAbilityType());

    if (bTryPrepare)
      StartPrepare();
	}
}

void UPestilenceAbilityComponent::OnDropPickable(UPickableComponent* Pickable, bool bLaunched)
{
  if (CurrentState) CurrentState->CancelPrepare();
  AbilityObjects[CurrentAbilityId]->Select(false);
	CurrentAbility = AbilityObjects[CurrentAbilityId];
  OnAbilityEquipped.Broadcast(CurrentAbility->GetAbilityType());

  if (bTryPrepare)
    StartPrepare();
}

void UPestilenceAbilityComponent::Initialize()
{
	APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
	if (!GameMode) return;

  AbilityObjects = GameMode->GetAbilities();
  PickableAbility = GameMode->GetPickableAbility();
  CurrentAbilityId = 0;
  for (int i = 0; i < AbilityObjects.Num(); ++i)
  {
    AbilityObjects[i]->SetNewOwner(Owner);
  }
  PickableAbility->SetNewOwner(Owner);

  MaxAbilities = GameMode->GetMaxAbilities();
  EquipAbility(CurrentAbilityId, false);

  GameMode->OnMaxAbilitiesChanged.AddDynamic(this, &UPestilenceAbilityComponent::OnMaxAbilitiesChanged);

	Owner->GetPestilencePossessionComponent()->OnEndPossessEvent().AddDynamic(this, &UPestilenceAbilityComponent::OnEndPossess);
}		

void UPestilenceAbilityComponent::OnEndPossess()
{
  Owner->GetInventoryComponent()->OnTakePickable.RemoveDynamic(this, &UPestilenceAbilityComponent::OnTakePickable);
  Owner->GetInventoryComponent()->OnDropPickable.RemoveDynamic(this, &UPestilenceAbilityComponent::OnDropPickable);
  Owner->GetPestilencePossessionComponent()->OnEndPossessEvent().RemoveDynamic(this, &UPestilenceAbilityComponent::OnEndPossess);

  if(Owner->GetPestilencePossessionComponent()->OnPossessed.IsAlreadyBound(this, &UPestilenceAbilityComponent::Initialize))
    Owner->GetPestilencePossessionComponent()->OnPossessed.RemoveDynamic(this, &UPestilenceAbilityComponent::Initialize);

	ChangeState(IdleState);

  APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
	if (!GameMode) return;
  GameMode->OnMaxAbilitiesChanged.RemoveDynamic(this, &UPestilenceAbilityComponent::OnMaxAbilitiesChanged);
}

void UPestilenceAbilityComponent::OnMaxAbilitiesChanged(int _MaxAbilities)
{
	MaxAbilities = _MaxAbilities;
	EquipAbility(MaxAbilities - 1);
}
