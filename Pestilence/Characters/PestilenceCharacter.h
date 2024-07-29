#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Perception/AISightTargetInterface.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Interface/PestilenceAIInterface.h"
#include "../Components/Abilities/ProjectilePredictionVision.h"
#include "SkeletalMergingLibrary.h"
#include "PestilenceCharacter.generated.h"

class ANPCManager;
class UTakeDownComponent;
class APestilenceAIController;
class UArchetypeComponent;
class UPlayerHUDWidget;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UInputAction;
class UInputMappingContext;
class UPestilenceMovementComponent;
class UPestilencePossessionComponent;
class UPestilenceVisionComponent;
class UInteractiveBaseComponent;
class AInteractiveActor;
class APickUpBase;
class USuspiciousActionSenderComponent;
class UPestilenceAbilityComponent;
class UPestilenceAbilityComponent;
class UPestilencePauseComponent;
class UInteractorComponent;
class USphereComponent;
class UInventoryComponent;
class UInteractableComponent;
class UMixAndMatchComponent;
class UNiagaraComponent;
class UMixAndMatchSkeletalMeshComponent;
class UFailStateComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillPlayer, bool, _bCrouched);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTalkRumour, bool, _bIsTalking);

UCLASS(config=Game)
class APestilenceCharacter : public ACharacter, public IAISightTargetInterface, public IPestilenceAIInterface
{
	GENERATED_BODY()

public:
	//TODO: Move this to MovementComponent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* LockpickingMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PossessionMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* KeyboardControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookGamepadAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	float MouseSensitivityX = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	float MouseSensitivityY = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	float ControllerSensitivityX = 0.7f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	float ControllerSensitivityY = 0.6f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	float LockpickCameraMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeanLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeanRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	float TriggerThreshold = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Interact", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Interact", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LaunchAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> CastAbilityAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> PrepareAbilityAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAbility1Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAbility2Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAbility3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAbility4Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAbilityNextAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAbilityPrevAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Meta", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PauseAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Meta", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> DiaryAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Scheme", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> KeyboardAction;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Scheme", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> ControllerAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Possession", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PestilenceVisionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Possession", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PestilencePossessionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Meta", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SaveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Meta", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LoadAction;

	//DEBUG FOR NOW SO THAT THE HEARING EFFECT OF THE AI CAN BE TRIED
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Possession", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PestilenceGenerateSoundAction;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence | Vision", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> HighlightDetected;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence | Vision", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> HighlightSelected;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence | Vision", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> HighlightUsing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence | Vision", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> HighlightForbidden;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<UFailStateComponent> FailStateComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Components) //Esta aqui por el BPRO 
		TObjectPtr<UMixAndMatchSkeletalMeshComponent> Head;
	
	//InteractiveActorsStuff
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AInteractiveActor> CurrentIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AInteractiveActor> CurrentPickUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> IAHolding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInteractiveBaseComponent> NPCBaseComponent;

	UPROPERTY(EditAnywhere)
	EPestilenceTypeCharacter PestilenceType = EPestilenceTypeCharacter::Pawn;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Mix n Match")
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;
	
  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Movement")
	FOnKillPlayer OnKillPlayer;
	
  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Movement")
	FOnTalkRumour OnTalkRumour;

	UFUNCTION(BlueprintCallable)
	APestilenceAIController* GetControllerAI() const;
	
	UFUNCTION(BlueprintCallable)
	void GenerateSound();

	UFUNCTION(BlueprintCallable)
	void DetachPickUp();
	
	//virtual void PossessedBy(AController* NewController) override;

	TObjectPtr<USkeletalMeshComponent> GetMesh1P() const { return Mesh1P; }
	TObjectPtr<USceneComponent> GetCameraPivotComponent() const { return CameraPivotComponent; }
	TObjectPtr<UCameraComponent> GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	TObjectPtr<USceneComponent> GetAbilityAttachment() const { return AbilityAttachment; }
	TObjectPtr<UPestilenceMovementComponent> GetPestilenceMovementComponent() const { return PestilenceMovementComponent; }
	TObjectPtr<UPestilencePossessionComponent> GetPestilencePossessionComponent() const { return PestilencePossessionComponent; }
	TObjectPtr<UPestilenceVisionComponent> GetPestilenceVisionComponent() const { return PestilenceVisionComponent; }
	TObjectPtr<USuspiciousActionSenderComponent> GetSuspiciousActionSenderComponent() const { return SuspiciousActionSenderComponent; }
	TObjectPtr<UPestilenceAbilityComponent> GetPestilenceAbilityComponent() const { return PestilenceAbilityComponent; }
	TObjectPtr<UPlayerHUDWidget> GetPlayerHUD() const { return PlayerHUD; }
	TObjectPtr<UPestilencePauseComponent> GetPauseComponent() const { return PauseComponent; }
	TObjectPtr<UArchetypeComponent> GetArchetypeComponent() const { return ArchetypeComponent; }
	TObjectPtr<UMaterialInstance> GetDissolveMaterial() const { return DissolveMaterial; }
	TObjectPtr<UInventoryComponent> GetInventoryComponent() const { return InventoryComponent; }
	TObjectPtr<USceneComponent> GetInventoryAttachment() const { return InventoryAttachment; }
	TObjectPtr<UInteractorComponent> GetInteractorComponent() const { return InteractorComponent; }
	TObjectPtr<UMixAndMatchComponent> GetMixAndMatchComponent() const { return MixAndMatchComponent; }
	TObjectPtr<UProjectilePredictionVision> GetProjectilePredictionVision() const { return ProjectilePredictionVision; }
	TObjectPtr<UMixAndMatchSkeletalMeshComponent> GetHead() const { return Head; }
	TObjectPtr<UMixAndMatchSkeletalMeshComponent> GetChest() const { return Chest; }
	TObjectPtr<UMixAndMatchSkeletalMeshComponent> GetLegs() const { return Legs; }
  TObjectPtr<UFailStateComponent> GetFailStateComponent() const { return FailStateComponent; }
  TObjectPtr<UTakeDownComponent> GetTakeDownComponent() const { return TakeDownComponent; }

	virtual EPestilenceTypeCharacter GetTypeCharacter() override { return PestilenceType; }
	
	void SetOverlayMaterialDetected();
	void SetOverlayMaterialSelected();
	void SetOverlayMaterialForbidden();
	void SetOverlayMaterialUsing();
	void SetOverlayMaterialNone();
	
  UFUNCTION(BlueprintCallable)
	void StartInfectParticles();
	UFUNCTION(BlueprintCallable)
	void StopInfectParticles();
  UFUNCTION(BlueprintCallable)
	void StartImpactParticles();
	
  UFUNCTION(BlueprintCallable)
	void StartTakeDownParticles();
  UFUNCTION(BlueprintCallable)
	void StopTakeDownParticles();

	UFUNCTION(BlueprintCallable)
	bool CanBeTakeDown() const { return bCanBeTakeDown; }
protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const override;

private:
	UPROPERTY(EditAnywhere, Category="Pestilence")
	FVector CanBeSeenFromOffset = FVector(0,0,50);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> CameraPivotComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

  UPROPERTY(VisibleAnywhere, Category = Components)
  TObjectPtr<USceneComponent> AbilityAttachment;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPestilenceMovementComponent> PestilenceMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPestilencePossessionComponent> PestilencePossessionComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPestilenceVisionComponent> PestilenceVisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USuspiciousActionSenderComponent> SuspiciousActionSenderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPestilenceAbilityComponent> PestilenceAbilityComponent;

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;

	UPROPERTY()
	TObjectPtr<UPlayerHUDWidget> PlayerHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPestilencePauseComponent> PauseComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArchetypeComponent> ArchetypeComponent;

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UMaterialInstance> DissolveMaterial;

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UInteractorComponent> InteractorComponent;
	
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<USphereComponent> InteractionSphere;

	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<USceneComponent> InventoryAttachment;

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	// Required components for recieve interactions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractableComponent> InteractableComponent;

	UPROPERTY(EditAnywhere, Category = "Components | Interactable")
	TObjectPtr<UCapsuleComponent> InteractableCapsuleComponent;

	// Take Down skill
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UTakeDownComponent> TakeDownComponent;

	//SkeletalMeshComponent Mix&Match
	UPROPERTY(EditAnywhere)
		TObjectPtr<USceneComponent> MixAndMatchRootComponent;

	UPROPERTY(VisibleAnywhere, Category = Components)
		TObjectPtr<UMixAndMatchSkeletalMeshComponent> Chest;
	UPROPERTY(VisibleAnywhere, Category = Components)
		TObjectPtr<UMixAndMatchSkeletalMeshComponent> Legs;
	
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UProjectilePredictionVision> ProjectilePredictionVision;
	
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> InfectParticles;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> ImpactParticles;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> LeftTakeDownParticles;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> RightTakeDownParticles;

	//protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UMixAndMatchComponent> MixAndMatchComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<APestilenceAIController> PestilenceAIController;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPestilenceAIComponent> AIComponent;

  UPROPERTY(EditAnywhere, Category = Components)
  bool bCanBeTakeDown = true;

	private:
	// Movement Input Callbacks
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LeanLeft(const FInputActionValue& Value);
	void LeanRight(const FInputActionValue& Value);
	void CrouchInput();
	void TryWalk(const FInputActionValue& Value);
	void TryWalkToggle();
	void Roll();

	// Possession Input Callbacks
	void StartPossession();
	void CancelPossession();
	void StartVision();
	void CancelVision();
	
	UFUNCTION()
	void OnEndPossessCallback();
	
	UFUNCTION()
	void OnPossessed();

	// Interact Input 
	void Interact();

	// Launch pickable Input
	void Launch();
	
	// Ability Input
	void StartCast();
	void CancelCast();
	void StartPrepare();
	void CancelPrepare();
	void SelectAbility1();
	void SelectAbility2();
	void SelectAbility3();
	void SelectAbility4();
	void SelectAbilityNext();
	void SelectAbilityPrev();

	// Add PauseMenu
	void AddPauseMenu();
	void AddDiaryMenu();

	void SetKeyboardControl();
	void SetControllerControl();

public:
	void ChangeToLockpinkMappingContext();
	void ChangeToDefaultMappingContext();
  UFUNCTION(BlueprintCallable)
  void ChangeMappingContext(UInputMappingContext* _NewMappingContext);
  UFUNCTION(BlueprintCallable)
  void AddMappingContext(UInputMappingContext* _NewMappingContext, int _Priority);
	UFUNCTION(BlueprintCallable)
  void RemoveMappingContext(UInputMappingContext* _MappingContextToRemove);
	
	UFUNCTION(BlueprintCallable)
	void AIInteract();

	//SelfSaveStuff
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void CheckSavedState();
	void OnKill();

	bool bIsDead = false;
	FVector OriginalLocation; // beginplayloc to load lvlstreaming 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Savegame")
	FGuid ID;

	APestilenceCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void OnConstruction
	(
		const FTransform& Transform
	) override;

	UFUNCTION()
	void QuickSaveAction();

	UFUNCTION()
	void QuickLoadAction();

private:

	TObjectPtr<ANPCManager> ManagerReference;
	
	UInputMappingContext* CurrentMappingContext;

	float LastLeanInput = 0.f;

	virtual void Tick(float DeltaSeconds) override;

	bool bUsingController = false;
	bool bInMinigame = false;
};

