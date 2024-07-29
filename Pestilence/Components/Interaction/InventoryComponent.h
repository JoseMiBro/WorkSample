#pragma once

#include "CoreMinimal.h"
#include "PickableComponent.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTakePickableSignature, UPickableComponent*, Pickable, bool, bResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDropPickableSignature, UPickableComponent*, Pickable, bool, bLaunched);

class UPickableComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PESTILENCE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// ----------- Events -----------

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FTakePickableSignature OnTakePickable;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FDropPickableSignature OnDropPickable;

	// ------------------------------
	
	UPROPERTY(EditAnywhere)
	float LaunchSpeed = 2000;
	
	UPROPERTY(EditAnywhere)
	float DropForceScale = 30000.f;

	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(USceneComponent* InventoryAttachment);

	UFUNCTION(BlueprintCallable)
	bool HasPickable() const { return CurrentPickable != nullptr; }

	UFUNCTION(BlueprintCallable)
	UPickableComponent* GetCurrentPickable() const { return CurrentPickable; }
	
  // This should be called from the pickable object in the OnInteract callback
  UFUNCTION(BlueprintCallable)
	void TakePickable(UPickableComponent* Pickable);

	UFUNCTION(BlueprintCallable)
	void DropPickable(bool bLaunch = false);

	UFUNCTION(BlueprintCallable)
	void ClearPickable();
	
private:
	UPROPERTY()
	TObjectPtr<UPickableComponent> CurrentPickable;

	UPROPERTY()
	TObjectPtr<USceneComponent> PickableAttachment;
	
	virtual void BeginPlay() override;
};