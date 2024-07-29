#include "ArchetypeComponent.h"
#include "../../Props/ArchetypeZone.h"

#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/MixAndMatch/MixAndMatchComponent.h"

UArchetypeComponent::UArchetypeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UArchetypeComponent::BeginPlay()
{
	Super::BeginPlay();
	//OnActorBeginOverlap
}

bool UArchetypeComponent::IsInCorrectZone() const { return bIsInCorrectZone; }
bool UArchetypeComponent::IsInForbiddenZone() const { return bIsInForbiddenZone; }

void UArchetypeComponent::ZoneChanged(bool _bIsInCorrectZone, bool _bIsInForbiddenZone)
{
  if (_bIsInCorrectZone == bIsInCorrectZone && _bIsInForbiddenZone == bIsInForbiddenZone) return;
  bIsInCorrectZone = _bIsInCorrectZone;
  bIsInForbiddenZone = _bIsInForbiddenZone;

	OnZoneChanged.Broadcast(bIsInCorrectZone, bIsInForbiddenZone);
}

const AArchetypeZone* UArchetypeComponent::GetCurrentZone() const
{
	return CurrentZone;
}

void UArchetypeComponent::SetCurrentZone(const AArchetypeZone* _CurrentZone)
{
	CurrentZone = _CurrentZone;
}

const AArchetypeZone* UArchetypeComponent::GetPreviousZone() const
{
  return PreviousZone;
}

void UArchetypeComponent::SetPreviousZone(const AArchetypeZone* _PreviousZone)
{
	PreviousZone = _PreviousZone;
}

#if WITH_EDITOR
void UArchetypeComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	//if (GetOwner()) 
	//{
	//	APestilenceCharacter* pCharacter = Cast<APestilenceCharacter>(GetOwner());
	//	if (pCharacter) 
	//	{
	//		UMixAndMatchComponent* pMix = pCharacter->GetMixAndMatchComponent();
	//		if (pMix) 
	//		{
	//			pMix->bFirstTime = false;
	//			pMix->OnButtonClick();
	//			
	//		}
	//	}
	//}
}
#endif
