#include "Pestilence/Components/AI/AIReactionComponent.h"
#include "Containers/Map.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

UAIReactionComponent::UAIReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAIReactionComponent::React_Implementation(APestilenceAIController* PestilenceAIController,
	EPestilenceTypeCharacter TypeCharacter)
{
}

void UAIReactionComponent::SetReactionMap(const TObjectPtr<APestilenceAIController>& Key, EPestilenceTypeCharacter Value)
{
	if (!ReactionMap.Contains(Key) && ReactionMap.Num() < MaxCharaterView)
	{
		ReactionMap.Add(Key, Value);
	}
}

void UAIReactionComponent::SetRemoveReactionMap(const TObjectPtr<APestilenceAIController>& Key)
{
	if (ReactionMap.Contains(Key)) 
	{
		ReactionMap.FindAndRemoveChecked(Key);
	}
}

void UAIReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	PestilenceCharacter = Cast<APestilenceCharacter>(GetOwner());

	if (PestilenceCharacter)
		OwnerAIController = Cast<APestilenceAIController>(PestilenceCharacter->GetController());
}

void UAIReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (const auto& Pair : ReactionMap)
	{
		if (Pair.Key) 
		{
			React_Implementation(Pair.Key, Pair.Value);
		}
	}
}

ECharacterType UAIReactionComponent::GetType() const
{
	return Type;
}