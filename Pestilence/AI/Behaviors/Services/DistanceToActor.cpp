#include "Pestilence/AI/Behaviors/Services/DistanceToActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"

UDistanceToActor::UDistanceToActor()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Distance To Actor");
	BlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDistanceToActor, BlackboardKey));
	BlackboardActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDistanceToActor, BlackboardActorKey), AActor::StaticClass());
}

void UDistanceToActor::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	const FName BlackboardKeyName = BlackboardKey.SelectedKeyName;
	const FName BlackboardActorKeyName = BlackboardActorKey.SelectedKeyName;

	const TObjectPtr<AActor> SelfActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Keys::Self));

	const TObjectPtr<AActor> ActorKey = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardActorKeyName));

	if (SelfActor == nullptr || ActorKey == nullptr)
		return;

	const float Distance = SelfActor->GetDistanceTo(ActorKey);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BlackboardKeyName, Distance);
}

FString UDistanceToActor::GetStaticServiceDescription() const
{
	return FString("Distance to other Actor");
}