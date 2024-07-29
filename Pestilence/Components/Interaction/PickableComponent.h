#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitCallbackCompleted);

class UInteractableComponent;
class UInteractorComponent;
class UProjectileMovementComponent;
class UShapeComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PESTILENCE_API UPickableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Pickable")
	FName PickableType = "None";
	
	UPROPERTY(EditAnywhere, Category = "Pickable")
	TArray<TEnumAsByte<ECollisionChannel>> IgnoreChannels = { ECC_Pawn };
	
	UPROPERTY(EditAnywhere, Category = "Pickable")
	TObjectPtr<USoundBase> PickSound;

	UPROPERTY(EditAnywhere, Category = "Pickable")
	bool bIsBreakable = true;

	UPROPERTY(EditAnywhere, Category = "Pickable")
	TObjectPtr<USoundBase> BreakSound;

	UPROPERTY(EditAnywhere, Category = "Pickable")
	float BreakNoiseRangeAI = 400.f;
	
	UPROPERTY(EditAnywhere, Category = "Pickable")
	float BreakSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Pickable")
	FVector MaxRotationSpeed = FVector(100.f, 50.f, 0.f);

  UPROPERTY(EditAnywhere, Category = "Pickable")
	FVector MinRotationSpeed = FVector(75.f, 0.f, 0.f);

	UPickableComponent();

	UFUNCTION(BlueprintCallable)
	void Throw(const FVector& ThrowSpeed);

	virtual void SetActive(bool bNewActive, bool bReset) override;
	UInteractableComponent* GetInteractable() const;

	UPROPERTY(BlueprintAssignable)
	FOnHitCallbackCompleted OnHitCallbackCompleted;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TObjectPtr<UInteractableComponent> Interactable;
	TObjectPtr<UShapeComponent> PhysicsShape;
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	bool bHasBeenHit = false;
	bool bThrowed = false;

	FVector RotationSpeed;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Initialize(UProjectileMovementComponent* ProjectileComponent, UShapeComponent* PhysicsShapeComponent);
	
	UFUNCTION()
	void OnInteracted(bool bResult, UInteractorComponent* Interactor);

	UFUNCTION()
    void OnComponentHitCallback(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void DestroyOnbreak();
};