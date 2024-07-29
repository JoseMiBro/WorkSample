#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

class USphereComponent;
class UInteractableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLookingInteractableChangedSignature, UInteractableComponent*, PreviousInteractable,
	UInteractableComponent*, CurrentInteractable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLookingInteractableSignature, UInteractableComponent*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTryInteractSignature, bool, bPerformed, bool, bSuccess, UInteractableComponent*, Interactable);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PESTILENCE_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// ----------- Events -----------
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FLookingInteractableChangedSignature OnLookingInteractableChanged;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FLookingInteractableSignature OnLookingInteractable;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FTryInteractSignature OnTriedToInteract;
	
	// ------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<USphereComponent> InteractSphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<USceneComponent> LookComponent;
	
	UInteractorComponent();
	
	UFUNCTION(BlueprintCallable)
	void Initialize(USphereComponent* InteractSphere, USceneComponent* LookSource);
	
	// Should be called by the interact input
	UFUNCTION(BlueprintCallable)
	bool TryInteract();

	UFUNCTION(BlueprintCallable)
	void SearchForNearbyInteractables();

	UFUNCTION(BlueprintCallable)
	UInteractableComponent* GetLookingInteractable() const { return CurrentInteractable; }
	
	UFUNCTION(BlueprintCallable)
	bool IsLookingAnInteractable() const { return CurrentInteractable != nullptr; }
	
	UFUNCTION(BlueprintCallable)
	void Enable(bool _Enable);
	
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
  float InteractionDistance = 200.f;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
  int NearbyInteractables = 0;

private:
	TObjectPtr<UInteractableComponent> CurrentInteractable;
	TObjectPtr<UInteractableComponent> PreviousInteractable;

	virtual void BeginPlay() override;
	virtual void Activate(bool bReset) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Deactivate() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	// Resets the search values
	void Reset();

	// Search looking interactables using line trace
	void SearchForLookingInteractable();

	void PollEvents();

	// If there are nearby interactables enable search
	UFUNCTION()
	void OnShapeBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// If there are not nearby interactables disable search
	UFUNCTION()
	void OnShapeEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	float InteractSphereRadius = 0.f;
};