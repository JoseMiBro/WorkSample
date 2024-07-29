// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pestilence/Components/AI/AIReactionComponent.h"
#include "PestilenceLookAtComponent.generated.h"


class UPestilenceSightComponent;
class UPestilenceCharacterAnimInstance;
class APestilenceAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UPestilenceLookAtComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPestilenceLookAtComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere, Category="Head Movement")
	FVector LookAtPoint;
	UPROPERTY(EditAnywhere, Category="Head Movement")
	FVector DefaultLookAtPointOffset;
	UPROPERTY(EditAnywhere, Category="Head Movement")
	float MaxMovementAngle;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APestilenceAIController> OwnerController;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPestilenceSightComponent> ControllerSightComponent;
	TObjectPtr<UAIReactionComponent> CharacterReactionComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPestilenceCharacterAnimInstance> AnimInstance;

	void TurnHead();
	void LookStraight();
};
