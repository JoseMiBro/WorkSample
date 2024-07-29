#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pestilence/InteractiveActors/InteractiveActor.h"
#include "Pestilence/Components/InteractiveActor/InteractiveBaseComponent.h"
#include "IActorVisionComponent.generated.h"

class APestilenceCharacter;
class UInteractiveBaseComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FeedBackTextDelegate, FString, FeedBackText);
DECLARE_DYNAMIC_DELEGATE(FeedBackTextDelegateExit);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UIActorVisionComponent : public UActorComponent
{
	GENERATED_BODY()
public: 
	FeedBackTextDelegate feedBackTextDelegate;
	FeedBackTextDelegateExit feedBackTextDelegateExit;

public:	
	UIActorVisionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "IActorVision")
	bool bShowDebugLineTrace = false;

	UPROPERTY(EditAnywhere, Category = "IActorVision")
	float bLineTraceDistance = 200.f;

	void PerformLineTrace();

	UPROPERTY()
	TObjectPtr<AActor> CurrentInteractiveActor;

	UPROPERTY()
	TObjectPtr<UInteractiveBaseComponent> CurrentInteractiveComp;

private:
	FString FeedBackText;
	TObjectPtr<APestilenceCharacter> Owner;
	AInteractiveActor* IAHit;
};
