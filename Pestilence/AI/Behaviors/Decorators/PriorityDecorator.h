// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "DynamicWeightFunctions/FunctionWrapper.h"
#include "PriorityDecorator.generated.h"

/**
 * 
 */

UCLASS()
class PESTILENCE_API UPriorityDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UPriorityDecorator(const FObjectInitializer& ObjectInitializer);

	/** Calculates the priority value of the associated behavior node. */
	virtual float CalculatePriorityValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	void ResetTimer(UBlackboardComponent* BlackboardComponent);
	UFUNCTION()
		float GetBasePriority() const;
	UFUNCTION()
		void SetBasePriority(float NewBasePriority);
	UFUNCTION()
		void SetTimerReset(float NewTimerReset);
	UFUNCTION()
		EPestilenceStates GetState() const;

protected:
	
	UPROPERTY(VisibleAnywhere, Category="BT Priority")
		float TimerReset = 2.f;
	
	UPROPERTY(EditAnywhere, Category="BT Priority")
		float BasePriority = 1.f;

	UPROPERTY(EditAnywhere, Instanced, Category = "BT Priority")
		TObjectPtr<UFunctionWrapper> DynamicWeightFunction;

	UPROPERTY(EditAnywhere, Instanced, Category = "BT Priority")
		TArray<TObjectPtr<UFunctionWrapper>> CheckForAbortFunctions;;
		
	UPROPERTY(EditAnywhere, Category="BT Priority")
		EPestilenceStates State;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
