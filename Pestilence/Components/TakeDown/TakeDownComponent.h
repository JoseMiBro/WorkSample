 #pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TakeDownComponent.generated.h"

class UInteractableComponent;
class UPestilenceMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakeDownStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakeDownFinishedSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PESTILENCE_API UTakeDownComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "TakeDown")
	float TakeDownTime = 2.f;
	UPROPERTY(EditAnywhere, Category = "TakeDown")
	float TakeDownDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "TakeDown")
	float LookAtTime = 0.2f;

	// ----------- Events -----------

	UPROPERTY(BlueprintAssignable, Category = "TakeDown")
	FTakeDownStartedSignature OnTakeDownStarted;

	UPROPERTY(BlueprintAssignable, Category = "TakeDown")
	FTakeDownFinishedSignature OnTakeDownFinished;

	// ------------------------------

	UTakeDownComponent();

private:
	TObjectPtr<UPestilenceMovementComponent> MovementComponent;

	float SelfStunTime = 0.f;
	bool ExecutingTakeDown = false;
	bool bWasCrouched = false;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	bool IsBehindInteractableActor(UInteractableComponent* Interactable) const;
	
	void EnableSelfStun();
	void DisableSelfStun();

	UFUNCTION()
	void OnTriedToInteractCallback(bool bPerformed, bool bSuccess, UInteractableComponent* Interactable);

	UFUNCTION()
	void OnLookingInteractableCallback(UInteractableComponent* Interactable);
};