// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCManager.h"

#include "PestilenceCharacter.h"
#include "VectorTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Components/MixAndMatch/MixAndMatchComponent.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/Components/Perception/PestilenceHearingComponent.h"
#include "Pestilence/Components/Perception/PestilenceLookAtComponent.h"
#include "Pestilence/Components/Perception/PestilenceSightComponent.h"

// Sets default values
ANPCManager::ANPCManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANPCManager::BeginPlay()
{
	Super::BeginPlay();
	NPCsOnLevel.SetNum(MaxNPCsOnLevel);
}

// Called every frame
void ANPCManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTime += DeltaTime;
	
	if(UpdateTime > UpdateTimeReset)
	{
		UpdateTime = 0;
		OptimizeNPCs();
	}
}

void ANPCManager::AddNPC(APestilenceCharacter* CharToAdd)
{
	for(int i = 0; i < NPCsOnLevel.Num(); ++i)
		if(!IsValid(NPCsOnLevel[i]))
		{
			NPCsOnLevel[i] = CharToAdd;
			break;
		}
}

void ANPCManager::RemoveNPC(APestilenceCharacter* CharToRemove)
{
	NPCsOnLevel.Remove(CharToRemove);
}

const TArray<TObjectPtr<APestilenceCharacter>>& ANPCManager::GetNpcs() const
{
	return NPCsOnLevel;
}

void ANPCManager::OptimizeNPCs()
{
	const TObjectPtr<APawn> PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(!IsValid(PlayerPawn)) return;
	TObjectPtr<APestilenceAIController> PawnController;
	const FVector PlayerPos = PlayerPawn->GetActorLocation();
	
	for (TObjectPtr<APestilenceCharacter> NPC : NPCsOnLevel)
	{
		if(!IsValid(NPC) || NPC->bIsDead) continue;
		PawnController = Cast<APestilenceAIController>(NPC->GetController());
		if(!IsValid(PawnController)) continue;
		TObjectPtr<APestilenceAIController> AIController = Cast<APestilenceAIController>(PawnController);
		if(!IsValid(AIController)) continue;
		const float DistanceToPlayer = UE::Geometry::Distance(NPC->GetActorLocation(),PlayerPos);
		if(DistanceToPlayer >= ActiveNPCsRadius)
		{
			if(PawnController->IsActorTickEnabled())
				PawnController->OnTickChange.Broadcast(ETickState::Disable);
			
			EnableNPC(NPC,PawnController,false,8);
			EnableController(AIController,0.0f,false);
			if(IsValid(AIController->GetBlackboard()))
			{
				AIController->Attention = 0.0f;
				AIController->GetBlackboard()->SetValueAsFloat(Keys::Alert,0.0f);
				AIController->GetBlackboard()->SetValueAsBool(Keys::LKPReached,true);
				AIController->GetBlackboard()->SetValueAsEnum(Keys::State,uint8(EPestilenceStates::Patrol));
				AIController->OnChangeState.Broadcast(EPestilenceStates::Patrol);
				Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent())->RestartTree();

			}
			
		}else if(DistanceToPlayer >= FunctionalNPCsRadius && DistanceToPlayer < ActiveNPCsRadius)
		{
			if(PawnController->GetActorTickInterval() == 0.f || !PawnController->IsActorTickEnabled())
				PawnController->OnTickChange.Broadcast(ETickState::Mid);
			
			UE::Math::TVector2<float> InputRange = UE::Math::TVector2<float>(FunctionalNPCsRadius,ActiveNPCsRadius);
			UE::Math::TVector2<float> OutputRange = UE::Math::TVector2<float>(MinTickTime,MaxTickTime);
			const float NewTickInterval = FMath::GetMappedRangeValueClamped(InputRange,OutputRange,DistanceToPlayer);
			NPC->SetActorTickInterval(NewTickInterval);
			
			PawnController->SetActorTickInterval(NewTickInterval);

			TObjectPtr<UMixAndMatchComponent> MixAndMatchComponent = NPC->FindComponentByClass<UMixAndMatchComponent>();
			EnableComponent(MixAndMatchComponent,NewTickInterval,true);
			TObjectPtr<UPestilenceMovementComponent> MovementComponent = NPC->FindComponentByClass<UPestilenceMovementComponent>();
			EnableComponent(MovementComponent,0.0f,true);
			NPC->FindComponentByClass<UAIPerceptionStimuliSourceComponent>()->SetComponentTickEnabled(true);

			EnableNPC(NPC,PawnController,true,4);
			EnableController(AIController,NewTickInterval,true);
			
		}else if(DistanceToPlayer < FunctionalNPCsRadius)
		{
			if(PawnController->GetActorTickInterval() != 0.f || !PawnController->IsActorTickEnabled())
				PawnController->OnTickChange.Broadcast(ETickState::Full);
			
			NPC->SetActorTickInterval(0.0f);
			//Test, UpdateRate, EvaluationRate, ThisTickDelta
			PawnController->SetActorTickInterval(0.0f);

			TObjectPtr<UMixAndMatchComponent> MixAndMatchComponent = NPC->FindComponentByClass<UMixAndMatchComponent>();
			EnableComponent(MixAndMatchComponent,0.0f,true);
			TObjectPtr<UPestilenceMovementComponent> MovementComponent = NPC->FindComponentByClass<UPestilenceMovementComponent>();
			EnableComponent(MovementComponent,0.0f,true);
			NPC->FindComponentByClass<UAIPerceptionStimuliSourceComponent>()->SetComponentTickEnabled(true);

			EnableNPC(NPC,PawnController,true,1);
			EnableController(AIController,0.0f,true);
		}
	}
}

void ANPCManager::EnableNPC(APawn* NPC, AController* NPC_Controller, bool Enable, int AnimTick) const
{
	APestilenceCharacter* PestilenceNPC = Cast<APestilenceCharacter>(NPC);
	PestilenceNPC->GetMixAndMatchComponent()->SetComponentTickEnabled(Enable);
	PestilenceNPC->GetPestilenceMovementComponent()->SetComponentTickEnabled(Enable);
	NPC->FindComponentByClass<UAIPerceptionStimuliSourceComponent>()->SetComponentTickEnabled(Enable);

	if(IsValid(PestilenceNPC->GetMesh()))
		if(PestilenceNPC->GetMesh()->AnimUpdateRateParams)
		{
			PestilenceNPC->GetMesh()->AnimUpdateRateParams->UpdateRate = AnimTick;
			PestilenceNPC->GetMesh()->AnimUpdateRateParams->EvaluationRate = AnimTick;
		}
	
	NPC->SetActorTickEnabled(Enable);
	NPC->SetActorHiddenInGame(!Enable);
	NPC->SetActorEnableCollision(Enable);

	if(IsValid(NPC_Controller))
		NPC_Controller->SetActorTickEnabled(Enable);
}
void ANPCManager::EnableController(APestilenceAIController* NPC_Controller, float Tick, bool Enable) const
{
	//En teoria no hace nada, pero prefiero cubrirme
	if(IsValid(NPC_Controller->GetBlackboard()))
		EnableComponent(NPC_Controller->GetBlackboard(),Tick,Enable);

	TObjectPtr<UPestilenceSightComponent> SightComponent = NPC_Controller->FindComponentByClass<UPestilenceSightComponent>();
	if(IsValid(SightComponent))
		EnableComponent(SightComponent,Tick,Enable);

	TObjectPtr<UPestilenceHearingComponent> HearingComponent = NPC_Controller->FindComponentByClass<UPestilenceHearingComponent>();
	if(IsValid(HearingComponent))
		EnableComponent(HearingComponent,Tick,Enable);
	
	TObjectPtr<UPestilenceLookAtComponent> LookAtComponent = NPC_Controller->FindComponentByClass<UPestilenceLookAtComponent>();
	if(IsValid(LookAtComponent))
		EnableComponent(LookAtComponent,Tick,Enable);

	TObjectPtr<UAIPerceptionComponent> PerceptionComponent = NPC_Controller->FindComponentByClass<UAIPerceptionComponent>();
	if(IsValid(PerceptionComponent))
	{
		EnableComponent(PerceptionComponent,Tick,Enable);
		PerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(),Enable);
	}
	
	TObjectPtr<UBrainComponent> BTComponent = NPC_Controller->FindComponentByClass<UBrainComponent>();
	if(IsValid(BTComponent))
		EnableComponent(BTComponent,Tick,Enable);
}

void ANPCManager::EnableComponent(UActorComponent* Component, float Tick, bool Enable) const
{
	Component->SetComponentTickEnabled(Enable);
	Component->SetComponentTickInterval(Tick);
}
