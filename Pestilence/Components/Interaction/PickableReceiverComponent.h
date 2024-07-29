#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickableReceiverComponent.generated.h"

class UInteractableComponent;
class UInteractorComponent;
class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPickableReceivedSignature, FName, PickableType, bool, bResult);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PESTILENCE_API UPickableReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// ----------- Events -----------

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FPickableReceivedSignature OnPickableReceived;
	
	UPROPERTY(EditAnywhere, Category = "Pickable Reciever")
	FName PickableType = "None";  
	
	UPickableReceiverComponent();

private:
	TObjectPtr<UInteractableComponent> Interactable;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteracted(bool bResult, UInteractorComponent* Interactor);
};