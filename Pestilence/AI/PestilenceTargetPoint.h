#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "PestilenceTargetPoint.generated.h"

UCLASS()
class PESTILENCE_API APestilenceTargetPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	APestilenceTargetPoint(const FObjectInitializer& ObjectInitializer);
	
	//Does nothing, just in case
	UPROPERTY(EditAnywhere, Category = "Target Point | Position")
		USceneComponent* Pivot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Point | Time")
		float WaitTime = 2.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Point | Animation")
		TObjectPtr<UAnimSequence> WaitingAnimation;
	
	void SetPointActive(bool _bPointActive);

	bool GetPointActive() const;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Point | Activation")
		bool bPointActive = true;
};