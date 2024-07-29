#include "PestilenceAIController.h"

#include "NetworkingDistanceConstants.h"
#include "Pestilence/Components/AI/PestilenceAIComponent.h"
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"

#include "Behaviors/blackboard_keys.h"
#include "BehaviorTree/BlackboardComponent.h"

//#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Perception/AIPerceptionComponent.h"

#include "UObject/UObjectGlobals.h"

#include "Kismet/GameplayStatics.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Enum.h>
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Pestilence/Components/AI/PestCrowdFollowingComponent.h"
#include "Pestilence/Components/AI/AIReactionComponent.h"
#include "Pestilence/Components/Perception/PestilenceSightComponent.h"
#include "Pestilence/Core/PestilenceGameMode.h"


APestilenceAIController::APestilenceAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPestCrowdFollowingComponent>("PathFollowingComponent"))
{
	Super::PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	ControllerType = EPestilenceControllerType::EStandarController;
}

void APestilenceAIController::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(GetPawn()))
	{
		PestilenceAIComponent = GetPawn()->FindComponentByClass<UPestilenceAIComponent>();
		if(!IsValid(PestilenceAIComponent)) return;
		BehaviorTree = PestilenceAIComponent->BehaviorTree;
	
		if (IsValid(BehaviorTree))
			InitBlackboard();
		ChangeOfStateReported = false;
		
		PestilencePlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	}
}

void APestilenceAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	TObjectPtr<UPestilenceSightComponent> SightComponent = FindComponentByClass<UPestilenceSightComponent>();
	if(IsValid(SightComponent))
		SightComponent->Init();

	PestilenceAIComponent = GetPawn()->FindComponentByClass<UPestilenceAIComponent>();
	if(!IsValid(PestilenceAIComponent)) return;
	BehaviorTree = PestilenceAIComponent->BehaviorTree;
	SetUpFrontiers();
	
	if (IsValid(BehaviorTree))
		InitBlackboard();
	
	UPestilencePossessionComponent* PossessionComponent = GetPawn()->FindComponentByClass<UPestilencePossessionComponent>();
	if(IsValid(PossessionComponent))
		if(!PossessionComponent->OnPossessed.IsAlreadyBound(this, &APestilenceAIController::SetPlayerKey))
			PossessionComponent->OnPossessed.AddDynamic(this, &APestilenceAIController::SetPlayerKey);
}

void APestilenceAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!IsValid(Blackboard)) return;
	const EPestilenceStates AuxState = static_cast<EPestilenceStates>(Blackboard->GetValueAsEnum(Keys::State));
	if(!ChangeOfStateReported)
	{
		ChangeOfStateReported = true;
		const EPestilenceStates AuxLastState = static_cast<EPestilenceStates>(Blackboard->GetValueAsEnum(Keys::LastState));
		if(AuxState == EPestilenceStates::Dead){ CurrentState = AuxState; return;}
		
		if( AuxLastState != AuxState  || AuxState == EPestilenceStates::Infected)
		{
			OnChangeState.Broadcast(CurrentState);
			const TObjectPtr<UPestilenceSightComponent> SightCmp = FindComponentByClass<UPestilenceSightComponent>();
			SightCmp->ChangeVisionConfig(AuxState);
		}

		if(AuxLastState != AuxState)
		{
			if(AuxState != EPestilenceStates::Alert)
				RestartOnChangeState(CurrentState);

			if(AuxLastState == EPestilenceStates::Escape)
			{
				if (Blackboard->GetValueAsBool(Keys::SomeOneInfected))
					Blackboard->SetValueAsBool(Keys::SomeOneInfected, false);
			}
			const APestilenceGameMode* GameMode = Cast<APestilenceGameMode>(GetWorld()->GetAuthGameMode());
			if (AuxState == EPestilenceStates::Chase)
			{
				GameMode->OnAIBeginChase.Broadcast();
			}
			else if (AuxLastState == EPestilenceStates::Chase)
			{
				GameMode->OnAIEndChase.Broadcast();
			}
		}
	}
	if (IsValid(Blackboard) && IsValid(PestilencePlayer) && IsValid(PestilencePlayer->GetPawn()))
	{
		Blackboard->SetValueAsVector(Keys::PlayerPosition, PestilencePlayer->GetPawn()->GetActorLocation());
	}
}

void APestilenceAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	PerceptionComponent->OnTargetPerceptionUpdated.RemoveAll(this);

	Super::EndPlay(EndPlayReason);

	// Added for lvlstreaming reasons by A.O.
	if (EndPlayReason == EEndPlayReason::Destroyed || EndPlayReason == EEndPlayReason::RemovedFromWorld)
	{
		const EPestilenceStates AuxState = static_cast<EPestilenceStates>(Blackboard->GetValueAsEnum(Keys::State));
		if (AuxState == EPestilenceStates::Chase)
		{
			const APestilenceGameMode* GameMode = Cast<APestilenceGameMode>(GetWorld()->GetAuthGameMode());
			GameMode->OnAIEndChase.Broadcast();
		}
}
}

void APestilenceAIController::UpdateAttention(EAttentionUpdateType TypeOfUpdate)
{
	if(CurrentState	== EPestilenceStates::Dead) return;
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	switch(TypeOfUpdate)
	{
		// If the sight asks to add visibility ...
		case EAttentionUpdateType::ESightUpdate:

			CalculateIntegrityFactor();
		
			if(bIsInForbiddenZone || (Blackboard && Blackboard->GetValueAsBool(Keys::Witness)))
			{
				// Till this moment the Visibility factor holds the player Visibility and the quality of the guard's vision
				
				Attention += VisibilityFactor * SightMultiplier * DeltaTime * GetStateMultiplier();
			}
			else
			{
				Attention += IntegrityFactor * VisibilityFactor * SightMultiplier * DeltaTime * GetStateMultiplier();
			}
			break;
		case EAttentionUpdateType::ESuspiciousActionUpdate:
			Attention += SuspiciousActionFactor;
			break;
		default:
			break;
			
	}

	// Clamp the value
	if(Attention < MinAttention ) Attention = MinAttention;

	if(Attention > MaxAttention) Attention = MaxAttention;
	
	//Pass new attention to the blackboard
	const TObjectPtr<UBlackboardComponent> ControllerBlackboardComponent = Blackboard;
	if (IsValid(ControllerBlackboardComponent)) 
	{
		ControllerBlackboardComponent->SetValueAsFloat(Keys::Alert, Attention);
		bool bPlayerHidden = ControllerBlackboardComponent->GetValueAsBool(Keys::PlayerHidden);
		if(!bPlayerHidden)
			ControllerBlackboardComponent->SetValueAsBool(Keys::SeeingPlayer, true);
		if(Attention >= 51.f)
			ControllerBlackboardComponent->SetValueAsBool(Keys::Warned, false);
	}
	
	// Reflect on the bar widget
	if(OnAttentionUpdate.IsBound())
		OnAttentionUpdate.Broadcast(Attention/MaxAttention);
}

void APestilenceAIController::UpdateAdviseAttention()
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	AdviseAttention += GeneralMultiplier * DeltaTime;

	if(AdviseAttention < MinAttention ) AdviseAttention = MinAttention;
	if(AdviseAttention > MaxAttention) AdviseAttention = MaxAttention;
}

void APestilenceAIController::CalculateIntegrityFactor()
{
	if(!IsValid(GetPawn())) return;
	const TObjectPtr<UAIReactionComponent> ReactionComponent = GetPawn()->FindComponentByClass<UAIReactionComponent>();
	if(!IsValid(ReactionComponent))return;
	const TObjectPtr<APawn> PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if(!IsValid(PlayerPawn)) return;
	const float Integrity = PlayerPawn->FindComponentByClass<UPestilencePossessionComponent>()->GetCurrentIntegrity();
	if(Integrity >= DetectionThreshold && ReactionComponent->GetType() != ECharacterType::Foreman)
	{
		IntegrityFactor = 0;
	}else
	{
		IntegrityFactor = 1;
	}
}

void APestilenceAIController::RemoveAttention(float AddAttention)
{
	Attention -= AddAttention;
	if(Attention < MinAttention ) Attention = MinAttention;

	const TObjectPtr<UBlackboardComponent> ControllerBlackboardComponent = Blackboard;
	if (IsValid(ControllerBlackboardComponent)) 
	{
		ControllerBlackboardComponent->SetValueAsFloat(Keys::Alert, Attention);
		//ControllerBlackboardComponent->SetValueAsBool(Keys::SeeingPlayer, false);
	}

	// Reflect on the bar widget
	if(OnAttentionUpdate.IsBound())
		OnAttentionUpdate.Broadcast(Attention/MaxAttention);
}

void APestilenceAIController::SetPlayerKey()
{
	if(CurrentState	== EPestilenceStates::Dead) return;
	
	for(TActorIterator<APestilenceAIController> ControllerItr(GetWorld());ControllerItr;++ControllerItr)
	{
		const TObjectPtr<APestilenceAIController> ControllerToCheck = *ControllerItr;
		if(!IsValid(ControllerToCheck->Blackboard)) continue;
		if(ControllerToCheck->Blackboard->GetValueAsBool(Keys::Witness))
			ControllerToCheck->OnWitnessChange.Broadcast(false);
		ControllerToCheck->Blackboard->SetValueAsBool(Keys::Witness, false);
	}

}

void APestilenceAIController::AddKey(UBlackboardData* Asset)
{
	if(bBlackboardInitialized) return;
	
	UBlackboardKeyType_Enum* State = Asset->UpdatePersistentKey<UBlackboardKeyType_Enum>("State");
	if (State != nullptr)
	{
		State->EnumName = TEXT("EPestilenceStates");
		State->EnumType = FindFirstObject<UEnum>(*(State->EnumName));
		State->bIsEnumNameValid = true;
	}
	
	UBlackboardKeyType_Enum* LastState = Asset->UpdatePersistentKey<UBlackboardKeyType_Enum>("LastState");
	if(LastState != nullptr)
	{
		LastState->EnumName = TEXT("EPestilenceStates");
		LastState->EnumType = FindFirstObject<UEnum>(*(LastState->EnumName));
		LastState->bIsEnumNameValid = true;
	}
	
	UBlackboardKeyType_Object* Player = Asset->UpdatePersistentKey<UBlackboardKeyType_Object>("Player");
	if (Player != nullptr)
		Player->BaseClass = APlayerController::StaticClass();
	
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("TargetPointPosition");
	Blackboard->SetValueAsVector(Keys::TargetPointPosition,FVector::Zero());
	
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("LastKnownPoint");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("PestilenceCharacterLKP");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("Rotate");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("RandomPoint");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("PlayerPosition");
	Blackboard->SetValueAsVector(Keys::PlayerPosition, FVector(0,0,0));
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("PlayerOrientation");
	Blackboard->SetValueAsVector(Keys::PlayerOrientation, FVector(0,0,0));
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("PlayerMovementDirection");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("WarningTargetPointPosition");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Vector>("WarningTargetPointRotation");
	
	Asset->UpdatePersistentKey<UBlackboardKeyType_Float>("WaitTime");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Float>("Timer");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Float>("StunTime");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Float>("Alert");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Float>("HeardSomethingOnPatrolWaitTime");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Float>("LoseInterestWaitTime");
	
	Asset->UpdatePersistentKey<UBlackboardKeyType_Int>("TargetPointIndex");
	
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("Static");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("Witness");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("InitialPosition");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("SeeingPlayer");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("CanGetAttention");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("LKPReached");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("UpdateTargetPoint");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("FirstTargetPointUpdate");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("MoveToWarningTargetPoint");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("IsRotating");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("PlayerKilled");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("IsChangingPoint");
	Asset->UpdatePersistentKey<UBlackboardKeyType_Bool>("HeardSomethingOnPatrol");
	Blackboard->SetValueAsBool(Keys::HeardSomethingOnPatrol, false);
	bBlackboardInitialized = true;
}

void APestilenceAIController::SetCurrentState(EPestilenceStates NewState)
{
	Blackboard->SetValueAsEnum(Keys::LastState, GetTypeHash(CurrentState));
	CurrentState = NewState;
	Blackboard->SetValueAsEnum(Keys::State, GetTypeHash(NewState));
}

void APestilenceAIController::RestartOnChangeState(EPestilenceStates NewState)
{
	if (!IsValid(Blackboard))
		return;
	
	Blackboard->SetValueAsBool(Keys::Warned, false);
	Blackboard->SetValueAsBool(Keys::SomeOneScaping, false);
	AdviseAttention = 0.f;
}

FName APestilenceAIController::StateToString(EPestilenceStates State)
{
	switch(State)
	{
		case EPestilenceStates::None:		return TEXT("None");	
		case EPestilenceStates::Patrol:		return TEXT("Patrol");	
		case EPestilenceStates::Doorman:	return TEXT("Doorman");	
		case EPestilenceStates::Alert:		return TEXT("Alert");	
		case EPestilenceStates::Distracted: return TEXT("Distracted");
		case EPestilenceStates::Escape:		return TEXT("Escape");	
		case EPestilenceStates::Advise:		return TEXT("Advise");	
		case EPestilenceStates::Infected:	return TEXT("Infected");
		case EPestilenceStates::Chase:		return TEXT("Chase");	
		case EPestilenceStates::Stun:		return TEXT("Stun");	
		case EPestilenceStates::Watchdog:	return TEXT("Watchdog");
		case EPestilenceStates::Dead:		return TEXT("Dead");	
		case EPestilenceStates::Interact:	return TEXT("Interact");
		default:							return TEXT("None");
	}
}

void APestilenceAIController::InitBlackboard()
{
	RunBehaviorTree(BehaviorTree);

	BlackboardDataAsset = Blackboard->GetBlackboardAsset();

	BlackboardDataAsset->OnUpdateKeys.AddUObject(this, &APestilenceAIController::AddKey);
		
	BlackboardDataAsset->OnUpdateKeys.Broadcast(BlackboardDataAsset);
		
	Blackboard->SetValueAsBool(Keys::Static, PestilenceAIComponent->bStatic);
	Blackboard->SetValueAsFloat(Keys::Alert, GetAttention());
	Blackboard->SetValueAsFloat(Keys::StunTime, BTPriorityConfig->FindRow<FBTPriorityConfigRow>(StateToString(EPestilenceStates::Stun),"")->MinStateTime);
	Blackboard->SetValueAsVector(Keys::PlayerMovementDirection, FVector(1,1,1));

	const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
		Blackboard->SetValueAsObject(Keys::Player, PlayerController);
	
	SetPlayerKey();
	Blackboard->SetValueAsBool(Keys::CanGetAttention, true);
	Blackboard->SetValueAsBool(Keys::UpdateTargetPoint, true);
	Blackboard->SetValueAsBool(Keys::MoveToWarningTargetPoint, false);
	Blackboard->SetValueAsBool(Keys::IsRotating, false);
	Blackboard->SetValueAsBool(Keys::PlayerKilled, false);
	Blackboard->SetValueAsBool(Keys::IsChangingPoint, false);
	
	CurrentState = PestilenceAIComponent->InitialState;
}

float APestilenceAIController::GetStateMultiplier() const
{
	float Multiplier = 0;

	switch(GetBlackboard()->GetValueAsEnum(Keys::State))
	{
		case EPestilenceStates::Patrol:	Multiplier = PatrolMultiplier;	break;
		case EPestilenceStates::Doorman:Multiplier = PatrolMultiplier;	break;
		case EPestilenceStates::Alert:	Multiplier = AlertMultiplier;	break;
		case EPestilenceStates::Chase:	Multiplier = ChaseMultiplier;	break;
		default: Multiplier = GeneralMultiplier;
	}

	return Multiplier;
}

void APestilenceAIController::SetUpFrontiers()
{
	PatrolInFrontier		= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Patrol),"")->InStateAttentionFrontier;
	PatrolOutFrontier		= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Patrol),"")->OutStateAttentionFrontier;
	AlertInFrontier			= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Alert),"")->InStateAttentionFrontier;
	AlertOutFrontier		= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Alert),"")->OutStateAttentionFrontier;
	ChaseInFrontier			= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Chase),"")->InStateAttentionFrontier;
	ChaseOutFrontier		= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Chase),"")->OutStateAttentionFrontier;
	AdviseInFrontier		= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Advise),"")->InStateAttentionFrontier;
	AdviseOutFrontier		= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Advise),"")->OutStateAttentionFrontier;
	EscapeInFrontier		= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Escape),"")->InStateAttentionFrontier;
	EscapeOutFrontier		= BTStateAttentionFrontiers->FindRow<FBaseStatesConfigRow>(StateToString(EPestilenceStates::Escape),"")->OutStateAttentionFrontier;
}
