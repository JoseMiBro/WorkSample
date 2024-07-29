// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIReactionComponent.generated.h"

class APestilenceAIController;
class APestilenceCharacter;
enum class EPestilenceTypeCharacter : uint8;

UENUM()
enum ECharacterType { Pawn,Guard,Foreman  };

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UAIReactionComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TMap<TObjectPtr<APestilenceAIController>, EPestilenceTypeCharacter> ReactionMap;

public:	
	UAIReactionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Reaction")
	void React(APestilenceAIController* PestilenceAIController, EPestilenceTypeCharacter TypeCharacter);
	virtual void React_Implementation(APestilenceAIController* PestilenceAIController, EPestilenceTypeCharacter TypeCharacter);
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<APestilenceCharacter> PestilenceCharacter;

	UPROPERTY(EditAnywhere)
		TObjectPtr<APestilenceAIController> OwnerAIController;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<APestilenceAIController> OtherAIController;

	UPROPERTY(EditAnywhere)
	EPestilenceTypeCharacter OtherType;

	UPROPERTY(EditAnywhere)
		int32 MaxCharaterView = 3;

	void SetReactionMap(const TObjectPtr<APestilenceAIController>& Key, EPestilenceTypeCharacter Value);

	void SetRemoveReactionMap(const TObjectPtr<APestilenceAIController>& Key);

	ECharacterType GetType() const; 
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY()                           
		TEnumAsByte<ECharacterType> Type; 
	};