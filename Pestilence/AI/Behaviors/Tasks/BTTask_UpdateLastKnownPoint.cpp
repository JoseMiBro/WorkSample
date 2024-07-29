#include "BTTask_UpdateLastKnownPoint.h"

#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UBTTask_UpdateLastKnownPoint::UBTTask_UpdateLastKnownPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Update Last Known Point";
}

EBTNodeResult::Type UBTTask_UpdateLastKnownPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());
	
	const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PestilenceAIController->GetBlackboard()->GetValueAsObject(Keys::Player));

	if (PestilenceAIController == nullptr || PlayerController == nullptr)
		return EBTNodeResult::Failed;
	
	PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, FVector::Zero());
	
	const FVector LastKnownPoint = PlayerController->GetPawn()->GetActorLocation();
	PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, LastKnownPoint);
	//DrawDebugSphere(GetWorld(), LastKnownPoint, 15.f, 10.f, FColor::Magenta, false, 4.f);

	return EBTNodeResult::Succeeded;
}

FString UBTTask_UpdateLastKnownPoint::GetStaticDescription() const
{
	return TEXT("Assing the Last Known Point");
}