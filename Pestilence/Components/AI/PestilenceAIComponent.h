#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/PestilenceTargetPoint.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "PestilenceAIComponent.generated.h"

class ARumourZone;
class AArchetypeZone;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UPestilenceAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPestilenceAIComponent();

	UPROPERTY(EditAnywhere, Category = "Pestilence | Variables")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Pestilence | Variables")
		TArray<APestilenceTargetPoint*> TargetPoints;
	UPROPERTY(EditAnywhere, Category = "Pestilence | Variables")
		TObjectPtr<APestilenceTargetPoint> WarningTargetPoint;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence | Variables")
	TArray<APestilenceCharacter*> PestilenceCharacters;

	UPROPERTY(EditAnywhere, Category = "Pestilence | Variables")
		TObjectPtr<AArchetypeZone> AssignedZone;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence | Variables")
		TObjectPtr<ARumourZone> RumourZone;
	
	UPROPERTY(EditAnywhere, Category = "Pestilence | Variables")
		bool bStatic = false;

	UPROPERTY(EditAnywhere, Category = "Pestilence | State")
	EPestilenceStates InitialState = EPestilenceStates::Patrol;

	UPROPERTY(EditAnywhere, Category = "Pestilence | Components")
		USphereComponent* SphereComponent;

protected:
	virtual void BeginPlay() override;	
};
