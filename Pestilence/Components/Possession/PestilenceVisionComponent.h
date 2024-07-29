#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PestilenceVisionComponent.generated.h"

class APestilenceCharacter;
class ANPCManager;

#define CC_PESTILENCE_CHARACTER ECC_GameTraceChannel2

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPestilenceVisionPerformed, bool, _Activated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPestilenceVisionTargetChanged, APestilenceCharacter*, _TargetCharacter);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UPestilenceVisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "PestilenceVision")
	FPestilenceVisionPerformed PestilenceVisionPerformedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "PestilenceVision")
	FOnPestilenceVisionTargetChanged OnPestilenceVisionTargetChangedDelegate;
	
	UPestilenceVisionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TObjectPtr<APestilenceCharacter> GetLookingCharacter() { return LookingCharacter; } 
	
protected:
	virtual void BeginPlay() override;
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	
private:
	UPROPERTY(EditAnywhere, Category = PestilenceVision)
	float DistanceToPossess = 1000.f;
	
	UPROPERTY(VisibleAnywhere, Category = PestilenceVision)
	TObjectPtr<APestilenceCharacter> LookingCharacter;
	
	UPROPERTY(EditAnywhere, Category = PestilenceVision)
	float RangeSearchFrec = 0.5f;

	UPROPERTY(EditAnywhere, Category = PestilenceVision)
	float MinimumAngleToLook = 0.3f;
	
	TObjectPtr<ANPCManager> NPCManager;
	TObjectPtr<APestilenceCharacter> Owner;
	TObjectPtr<APestilenceCharacter> PrevLookingCharacter;
	TArray<APestilenceCharacter*> PestilenceCharacters;
	TArray<APestilenceCharacter*> AvailableCharacters;
	float TimeSinceLastSearch = 0.f;
	
	UFUNCTION(BlueprintCallable)
	void GetAllPestilenceCharacters();
	void SearchForCharactersInRange();
	bool IsCharacterVisible(const APestilenceCharacter* Character);
	void SearchLookingCharacter();
	void RestoreSearchDefaults();
	void ApplyMaterials();
};
