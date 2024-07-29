#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/InteractableType.h"
#include "InteractableComponent.generated.h"

class UInteractorComponent;
class UMaterialInstance;

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FInteractDelegateSignature, UInteractorComponent*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractedSignature, bool, bResult, UInteractorComponent*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoverBeginSignature, UInteractorComponent*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoverStaySignature, UInteractorComponent*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoverEndSignature, UInteractorComponent*, Sender);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PESTILENCE_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// ----------- Events -----------

	UPROPERTY(BlueprintCallable,BlueprintAssignable, Category = "Interaction")
	FOnInteractedSignature OnInteracted;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnHoverBeginSignature OnHoverBegin;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnHoverStaySignature OnHoverStay;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnHoverEndSignature OnHoverEnd;

	// ------------------------------

	// This delegate should be bounded in order to evaluate the interaction
	UPROPERTY()
	FInteractDelegateSignature InteractDelegate;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bEvaluateFromBlueprint = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UShapeComponent> InteractShapeComponent;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TArray<TObjectPtr<AActor>> ActorsToHighlight;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TObjectPtr<UMaterialInstance> IsCloseMaterial;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TObjectPtr<UMaterialInstance> IsInteractableMaterial;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
  EInteractableType InteractableType;

	UInteractableComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(UShapeComponent* InteractShape);
	
	virtual bool Interact(UInteractorComponent* Sender);

	UFUNCTION()
	virtual void HoverBegin(UInteractorComponent* Sender);

	UFUNCTION()
	virtual void HoverStay(UInteractorComponent* Sender);
	
	UFUNCTION()
	virtual void HoverEnd(UInteractorComponent* Sender);
	
	UFUNCTION(BlueprintImplementableEvent)
	bool EvaluateInteraction(UInteractorComponent* Sender);

	virtual void SetActive(bool bNewActive, bool bReset) override;

  void SetIsCloseMaterialOverlay();
  void SetIsInteractableMaterialOverlay();
  void SetNoMaterialOverlay();

  bool IsCloseMaterialOverlaySet();
  bool IsIsInteractableMaterialOverlaySet();
  bool IsNoMaterialOverlaySet();

	bool ContainsSameMesh(UInteractableComponent* OtherInteractable);

	void SetCanBeInteracted(bool _bCanBeInteracted); 
	bool GetCanBeInteracted() const;

private:
	virtual void BeginPlay() override;

	TArray<TObjectPtr<UMeshComponent>> MeshComponents;
	bool bCanBeInteracted = true;

};