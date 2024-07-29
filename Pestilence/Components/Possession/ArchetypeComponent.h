#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pestilence/Enums/Archetype.h"
#include "Pestilence/Enums/Gender.h"
#include "ArchetypeComponent.generated.h"

class AArchetypeZone;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnZoneChanged, bool, _bIsInCorrectZone, bool, _bIsInForbiddenZone);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PESTILENCE_API UArchetypeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UArchetypeComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	EArchetype GetPestilenceArchetype() const { return Archetype; }

	UFUNCTION(BlueprintCallable)
	EGender GetPestilenceGender() const { return Gender; }

	UFUNCTION(BlueprintCallable)
	void SetPestilenceArchetype(const EArchetype NewArchetype) { Archetype = NewArchetype; }

	UFUNCTION(BlueprintCallable)
	void SetPestilenceGender(const EGender NewGender) { Gender = NewGender; }
	
	bool IsInCorrectZone() const;
	bool IsInForbiddenZone() const;
	void ZoneChanged(bool _bIsInCorrectZone, bool _bIsInForbiddenZone);

  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Archetype")
  FOnZoneChanged OnZoneChanged;

  const AArchetypeZone* GetCurrentZone() const;
  void SetCurrentZone(const AArchetypeZone* _CurrentZone);
  const AArchetypeZone* GetPreviousZone() const;
  void SetPreviousZone(const AArchetypeZone* _PreviousZone);

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif 

private:
	UPROPERTY(EditAnywhere)
	EArchetype Archetype;
	UPROPERTY(EditAnywhere)
	EGender Gender;

	const AArchetypeZone* CurrentZone;
	const AArchetypeZone* PreviousZone;

	bool bIsInCorrectZone = true;
	bool bIsInForbiddenZone = false;
};