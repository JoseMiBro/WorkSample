#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IA_SwingDoor.generated.h"

class UNavArea;
class UNavModifierComponent;
class UPestilenceNavLinkComponent;
class UCurveFloat;
class UUserWidget;
class USoundBase;
class ULockpickWidget;
class UInteractableComponent;
class UBoxComponent;
class USmartObjectComponent;
class UAnimInstance;
class APestilenceCharacter;
class UArrowComponent;
class UPestilenceMovementComponent;
class UNiagaraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnlocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitDoor);

UCLASS()
class PESTILENCE_API AIA_SwingDoor : public AActor
{
	GENERATED_BODY()
public:
	AIA_SwingDoor();

protected:
	virtual void BeginPlay() override;

	/* AI */
	UPROPERTY(EditAnywhere, Category = "AI|NavArea")
	TSubclassOf<UNavArea> InteractNavArea;

	UPROPERTY(EditAnywhere, Category = "AI|NavArea")
	TSubclassOf<UNavArea> NullNavArea;

	UPROPERTY(EditAnywhere, Category = "AI|NavLinkComponent")
	TObjectPtr<UPestilenceNavLinkComponent> PestilenceNavLinkComponent;

	//UPROPERTY(EditAnywhere, Category = "AI|Nav Modifier")
	//TObjectPtr<UNavModifierComponent> NavModifierComponent;

	UPROPERTY(EditAnywhere, Category = "AI|SmartObject")
	TObjectPtr<USmartObjectComponent> SmartObjectComponent;

public:
	virtual void Tick(float DeltaTime) override;

	/* AI */
	UPestilenceNavLinkComponent* GetPestilenceNavLinkComponent() const { return PestilenceNavLinkComponent; }

	//UNavModifierComponent* GetNavModifierComponent() const { return NavModifierComponent; }
	USmartObjectComponent* GetSmartObjectComponent() const { return SmartObjectComponent; }

	UFUNCTION()
	void OnTranslating(bool _bIsTranslating, UPestilenceMovementComponent* _MovementComponent);
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Door;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Handle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> LockpickPosFront;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> LockpickPosBack;
	UPROPERTY()
	TObjectPtr<UArrowComponent> LockpickArrowFront;
	UPROPERTY()
	TObjectPtr<UArrowComponent> LockpickArrowBack;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> EndLockpickPosFront;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> EndLockpickPosBack;
	UPROPERTY()
	TObjectPtr<UArrowComponent> EndLockpickArrowFront;
	UPROPERTY()
	TObjectPtr<UArrowComponent> EndLockpickArrowBack;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	float LockpickHalfHeight = 60.f;
	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	float LockpickLookAngleLimit = 20.f;


	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> DoorFrame;

	// This curve determines the Angle/Time of the door
	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	TObjectPtr<UCurveFloat> DoorCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLocked = false;

	//Dynamic makes the door open relatively to the position of the player
	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	bool bDynamic = false;

	// This curve determines the Animation of the Gate door
	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	TObjectPtr<UCurveFloat> GateCurve;

	//Gate Mode only opens from one side (DebugArrow)
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pestilence|Door")
  bool bGate = false;

  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pestilence|Door")
  bool bStartOpen = false;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	TObjectPtr<USoundBase> DoorOpeningSound;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	TObjectPtr<USoundBase> DoorClosingSound;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	TObjectPtr<USoundBase> DoorUnlockSound;

	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	TObjectPtr<USoundBase> GateWrongSideSound;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> SmokeParticles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Navigation|NavLinkPoints")
	FVector LinkStart = FVector3d(35.f, 100.f, 0.f);
	UPROPERTY(EditAnywhere, Category = "Navigation|NavLinkPoints")
	FVector LinkEnd = FVector3d(-35.f, 100.f, 0.f);
	UPROPERTY(EditAnywhere, Category = "Navigation|Obstacle")
	FVector ObstacleExtent = FVector(1.f, 25.f, 0.f);
	UPROPERTY(EditAnywhere, Category = "Navigation|Obstacle")
	FVector ObstacleOffset = FVector(0.f, 60.f, 0.f);

	UPROPERTY(VisibleAnywhere)
	float InitialYawRotation;
	
	UFUNCTION()
	void ControlDoor(float _Timer, UCurveFloat* _Curve);
	
	UFUNCTION()
	void SetState();

  UFUNCTION()
  void OnInteracted(bool bResult, UInteractorComponent* Sender);

	void DoorLogic(UInteractorComponent* Sender);

	void OneWayGateLogic(UInteractorComponent* Sender);
	
	UFUNCTION(BlueprintCallable)
	void SetOpenInmediate();
	
	UFUNCTION(BlueprintCallable)
	void SetClosedInmediate();

	UFUNCTION()
	void TriggerLockpickMinigame();

	UFUNCTION(BlueprintCallable)
	void UnlockDoorSuccess();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnlockDoorFailure();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UnlockDoorFailureBP();

	UPROPERTY(BlueprintAssignable, Category = "Pestilence|Door")
	FOnInitDoor ReStart;

	UFUNCTION(BlueprintImplementableEvent)
	void RestartMiniGame();

	UFUNCTION(BlueprintCallable)
	void UpdateNavMesh();
	
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Door", meta = (AllowPrivateAccess = "true"))
	bool bOpen;
	
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Door", meta = (AllowPrivateAccess = "true"))
	bool bShouldSaveState = true;

  UPROPERTY(BlueprintReadOnly, Category = "Pestilence|Door", meta = (AllowPrivateAccess = "true"))
	bool bReadyState;
	float RotateValue;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
	float ChangeContextDelay = 0.1f;

	float DoorTimer = 0.f;
	float DoorTime; 
	float GateTimer = 0.f;
	float GateTime;
	bool bUpdateDoor = false;
	bool bUpdateGate = false;
	bool bDoorReverse = false;
	bool bPickingDoor = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Door")
	bool bHasMinigame = false;

	TObjectPtr<APestilenceCharacter> Interactor;

  UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
  float PickingDoorNotifyTime = 0.3f;
	float PickingDoorNotifyTimer = 0.f;

  UPROPERTY(EditAnywhere, Category = "Pestilence|Door")
  TObjectPtr<UInteractableComponent> InteractableComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UBoxComponent> InteractableCollider;

	UFUNCTION(BlueprintImplementableEvent, Category = "NavEvent")
		void OnStartLink();

	UFUNCTION(BlueprintImplementableEvent, Category = "NavEvent")
		void OnEndLink();

	void DelayContext();

  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Door")
	FOnUnlocked OnUnlocked;
private:
	UPROPERTY()
	FTimerHandle MappingContextHandle;

	void CheckPawns(const FVector& _NewPawnLocation);
public:
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
	void EnableIdleBorderDoorknob();

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
	void DisableIdleBorder();

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
	void EnableDisintegrate();

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
	void EnableSuccess();

	UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
		void EnableFail();

  UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
  void StartSmokeParticles();
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
  void StopSmokeParticles();
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
  void StartImpactParticles();
  UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
  void StopImpactParticles();
	UFUNCTION(BlueprintCallable, Category = "Pestilence|Door")
	void SetLockedDoor(bool _bLocked);

  //SelfSaveStuff
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
  void CheckDoorSave();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Savegame")
	  FGuid ID;

  AIA_SwingDoor(const FObjectInitializer& ObjectInitializer);
  virtual void OnConstruction
  (
	  const FTransform& Transform
  ) override;

};
