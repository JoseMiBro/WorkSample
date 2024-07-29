#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Engine/DataTable.h"
#include "PestilenceAIController.generated.h"

class UPestilenceSightComponent;
class UAISenseConfig_Sight;
class UAISenseConfig;
class UPestilenceAIComponent;
class UBehaviorTree;
class UBlackboardComponent;

UENUM(BlueprintType)
enum class EPestilenceStates : uint8
{
	None		UMETA(DisplayName = "None"),
	Patrol		UMETA(DisplayName = "Patrol"), 
	Doorman		UMETA(DisplayName = "Doorman"), 
	Alert		UMETA(DisplayName = "Alert"), 
	Distracted	UMETA(DisplayName = "Distracted"), 
	Escape		UMETA(DisplayName = "Escape"),
	Advise		UMETA(DisplayName = "Advise"),
	Infected	UMETA(DisplayName = "Infected"),
	Chase		UMETA(DisplayName = "Chase"),
	Stun		UMETA(DisplayName = "Stun"), 
	Watchdog	UMETA(DisplauName = "Watchdog"),
	Dead		UMETA(DisplauName = "Dead"), 
	Interact	UMETA(DisplauName = "Interact"), 
};

USTRUCT(BlueprintType)
struct PESTILENCE_API FBTPriorityConfigRow : public FTableRowBase
{
	//GENERATED_USTRUCT_BODY()
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="BlackboardData")
	float StatePriority;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="BlackboardData")
	float MinStateTime;
};

USTRUCT(BlueprintType)
struct PESTILENCE_API FBaseStatesConfigRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="BlackboardData")
		float InStateAttentionFrontier;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="BlackboardData")
		float OutStateAttentionFrontier;
};

enum EAttentionUpdateType
{
	ESightUpdate,
	ESuspiciousActionUpdate
};

UENUM()
enum EPestilenceControllerType
{
	EStandarController,
	EDoormanController
};

UENUM(BlueprintType)
enum class SoundType : uint8
{
	BottleBraking,
	Step,
	Infection
};

UENUM(BlueprintType)
enum class ETickState : uint8
{
	Disable,
	Mid,
	Full
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeTick,ETickState,TickState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttentionUpdate,float,Awareness);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeState,EPestilenceStates,NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeWitness,bool,IsWitness);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCustomSoundPlayed,SoundType,SoundPlayed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStunReaplied);

UCLASS()
class PESTILENCE_API APestilenceAIController : public AAIController
{
	GENERATED_BODY()

public:
	//Unreal defaults
	APestilenceAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	
	//Blackboard Handling
	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<UPestilenceAIComponent> PestilenceAIComponent;
	UPROPERTY(EditAnywhere) //BehaviorTree Asset
		TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY(EditAnywhere) //Data Asset (Blackboard Asset Unreal)
		TObjectPtr<UBlackboardData> BlackboardDataAsset;

	UPROPERTY(EditAnywhere, Category="State Table Config")
		TObjectPtr<UDataTable> BTPriorityConfig;

	UPROPERTY(EditAnywhere, Category="State Table Config")
		TObjectPtr<UDataTable> BTStateAttentionFrontiers;
	
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float PatrolInFrontier;
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float PatrolOutFrontier;
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float AlertInFrontier;
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float AlertOutFrontier;
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float ChaseInFrontier;
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float ChaseOutFrontier;
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float AdviseInFrontier;
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float AdviseOutFrontier;
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float EscapeOutFrontier;
	UPROPERTY(VisibleAnywhere, Category="State Table Config")
		float EscapeInFrontier;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> SoundToPlayOnInfection;
	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> SoundToPlayOnLostInterest;
	
	UFUNCTION()
		void SetPlayerKey();
	UFUNCTION()
		void AddKey(UBlackboardData* Asset);
	UFUNCTION(BlueprintCallable)
		EPestilenceStates GetCurrentState() {return CurrentState;}
	UPROPERTY(VisibleAnywhere)
		bool ChangeOfStateReported;
	UPROPERTY(BlueprintAssignable)
		FChangeState OnChangeState;
	UPROPERTY(BlueprintAssignable)
		FStunReaplied OnStunReaplied;
	UPROPERTY(BlueprintAssignable)
		FChangeWitness OnWitnessChange;
	UPROPERTY(BlueprintAssignable)
		FCustomSoundPlayed OnLostInterest;
		
	UFUNCTION(BlueprintCallable)
		void SetCurrentState(EPestilenceStates NewState);
	UBlackboardComponent* GetBlackboard() const {return Blackboard;}
	
	//Attention Handling
	UPROPERTY(BlueprintAssignable)
		FAttentionUpdate OnAttentionUpdate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence Attention")
		float Attention = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence Attention")
		float AdviseAttention = 0.f;
	
	void UpdateAttention(EAttentionUpdateType TypeOfUpdate);
	void UpdateAdviseAttention();
	void RemoveAttention(float Attention);
	void SetMaxIntegrity(const float Value) { MaxIntegrity = Value; }
	void SetActualIntegrity(const float Value) { ActualIntegrity = Value; }
	float GetAttention() const { return Attention;}
	
	bool GetIsInForbiddenZone() const { return bIsInForbiddenZone;}
	void SetIsInForbiddenZone(const bool isInForbidden){ bIsInForbiddenZone = isInForbidden;}

	float GetIntegrityFactor() const { return IntegrityFactor;}
	void SetIntegrityFactor(const float NewIntegrityFactor){ IntegrityFactor = NewIntegrityFactor;}

	float GetVisibilityFactor() const { return VisibilityFactor;}
	void SetVisibilityFactor(const float NewVisibilityFactor){ VisibilityFactor = NewVisibilityFactor;}

	float GetSuspiciousActionFactor() const { return SuspiciousActionFactor;}
	void SetSuspiciousActionFactor(const float NewSuspiciousActionFactor){ SuspiciousActionFactor = NewSuspiciousActionFactor;}

	UFUNCTION(BlueprintCallable)
	void RestartOnChangeState(EPestilenceStates NewState);
	
	// Utilities
	static FName StateToString(EPestilenceStates State); 
	EPestilenceControllerType GetControllerType() const{ return ControllerType;}
	
	UFUNCTION()
		void CalculateIntegrityFactor();

	UPROPERTY(EditAnywhere) //Current State : It used in the BehaviorTree to select the tree
		EPestilenceStates CurrentState = EPestilenceStates::Patrol;

	UPROPERTY(EditAnywhere, Category = "Movement")
		bool bIsCrouched = false;

	UPROPERTY(BlueprintAssignable)
		FChangeTick OnTickChange;
	
private:
	
	//Detection Handling
	UPROPERTY(VisibleAnywhere,Category="Pestilence Perception")
		float IntegrityFactor = 0.f;
	UPROPERTY(VisibleAnywhere,Category="Pestilence Perception")
		float VisibilityFactor = 0.f;
	UPROPERTY(VisibleAnywhere,Category="Pestilence Perception")
		float SuspiciousActionFactor = 0.f;
	
	UPROPERTY(VisibleAnywhere,Category="Pestilence Perception")
		TObjectPtr<AActor> PlayerActor;
	
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<APlayerController> PestilencePlayer;

	bool bBlackboardInitialized = false;

	UFUNCTION()
		void InitBlackboard();
	UFUNCTION()
		float GetStateMultiplier() const;
	UFUNCTION()
		void SetUpFrontiers();

protected:
	//Attention Config
	UPROPERTY(EditAnywhere, Category = "Pestilence Attention")
		float MinAttention = 0.f;
	UPROPERTY(EditAnywhere, Category = "Pestilence Attention")
		float MaxAttention = 100.f;
	UPROPERTY(EditAnywhere, Category = "Pestilence Attention")
		float DetectionThreshold = 25.f;
	UPROPERTY(VisibleAnywhere,Category="Pestilence Attention")
		bool bIsInForbiddenZone = false;
	
	// Multipliers
	UPROPERTY(EditAnywhere, Category="Pestilence Multipliers")
		float SightMultiplier = 1.f;
	UPROPERTY(EditAnywhere, Category="Pestilence Multipliers")
		float PatrolMultiplier = 1.f;
	UPROPERTY(EditAnywhere, Category="Pestilence Multipliers")
		float AlertMultiplier = 1.f;
	UPROPERTY(EditAnywhere, Category="Pestilence Multipliers")
		float ChaseMultiplier = 1.f;
	UPROPERTY(EditAnywhere, Category="Pestilence Multipliers")
		float GeneralMultiplier = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Pestilence Integrity")
		float MaxIntegrity = 100.f;
	UPROPERTY(EditAnywhere, Category="Pestilence Integrity")
		float ActualIntegrity = 40.f;

	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<EPestilenceControllerType> ControllerType;

	//Unreal defaults
	UFUNCTION()
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
};