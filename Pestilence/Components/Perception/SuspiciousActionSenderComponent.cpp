#include "SuspiciousActionSenderComponent.h"

USuspiciousActionSenderComponent::USuspiciousActionSenderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USuspiciousActionSenderComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USuspiciousActionSenderComponent::SubmitSuspiciousAction(const TObjectPtr<AActor>& SenderActor,
                                                              const ESuspiciousActionType SuspiciousActionType)
{
	FSuspiciousAction Action{SuspiciousActionType};
	if (ensureMsgf(SuspiciousActionPoints.Contains(SuspiciousActionType),
		TEXT("You're trying to send a non registered Suspicious action.")))
	{
		Action.Points = SuspiciousActionPoints[SuspiciousActionType];
		SuspiciousActionPerformed.Broadcast(Action);
	}
}

void USuspiciousActionSenderComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}