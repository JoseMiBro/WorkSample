#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "InteractiveBaseComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UInteractiveBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractiveBaseComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	virtual void DefaultInteraction(APestilenceCharacter* CharActor);

	UPROPERTY(EditAnywhere, Category = "NPC")
	bool bIsInteractuable = false;


protected:
	virtual void BeginPlay() override;

	TObjectPtr<AActor> Owner;		
};
