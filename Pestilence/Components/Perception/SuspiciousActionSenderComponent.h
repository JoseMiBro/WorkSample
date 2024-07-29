#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pestilence/Components/Perception/SuspiciousAction.h"
#include "SuspiciousActionSenderComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSuspiciousActionPerformed, const FSuspiciousAction&, SuspiciousAction);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PESTILENCE_API USuspiciousActionSenderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspicious Actions")
	TMap<ESuspiciousActionType, float> SuspiciousActionPoints;

	USuspiciousActionSenderComponent();

	UFUNCTION()
	FSuspiciousActionPerformed& OnSuspiciousActionPerformed() { return SuspiciousActionPerformed; }

	void SubmitSuspiciousAction(const TObjectPtr<AActor>& SenderActor, ESuspiciousActionType SuspiciousActionType);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintAssignable, Category = "Suspicious Actions")
	FSuspiciousActionPerformed SuspiciousActionPerformed;	
};
