#include "BTTask_PassByHidingSpot.h"

#include "Kismet/GameplayStatics.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/Components/Perception/PestilenceSightComponent.h"
#include "Pestilence/InteractiveActors/IActors/IA_HidingSpot.h"

UBTTask_PassByHidingSpot::UBTTask_PassByHidingSpot(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Pass by hiding spot";
}

EBTNodeResult::Type UBTTask_PassByHidingSpot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<APestilenceAIController> PestilenceAIController = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());
	const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PestilenceAIController->GetBlackboard()->GetValueAsObject(Keys::Player));

	const TObjectPtr<UPestilenceSightComponent> SightComponent = PestilenceAIController->FindComponentByClass<UPestilenceSightComponent>();
	if(PlayerController == nullptr) return EBTNodeResult::Succeeded;
	if(SightComponent == nullptr) return EBTNodeResult::Succeeded;
	
	const TObjectPtr<APestilenceCharacter> PlayerCharacter = Cast<APestilenceCharacter>(PlayerController->GetPawn());

	if (PlayerCharacter == nullptr) return EBTNodeResult::Succeeded;
	
	if(!SightComponent->bOnSight)
	{
		const TObjectPtr<UPestilenceMovementComponent> MovementComponent = PlayerCharacter->GetPestilenceMovementComponent();
		if(MovementComponent && MovementComponent->IsHidden())
		{
			const UBoxComponent* Box1 = MovementComponent->GetHidingSpot()->GetPassByArea1();
			const UBoxComponent* Box2 = MovementComponent->GetHidingSpot()->GetPassByArea2();
			const FVector PosBox1 = UKismetMathLibrary::TransformLocation(Box1->GetOwner()->GetActorTransform(),Box1->GetRelativeLocation());
			const FVector PosBox2 = UKismetMathLibrary::TransformLocation(Box2->GetOwner()->GetActorTransform(),Box2->GetRelativeLocation());

			const float DistBox1 = FVector::DistSquared(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(),PosBox1);
			const float DistBox2 = FVector::DistSquared(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(),PosBox2);

			FVector NewLastKnownPoint;
			
			if(DistBox1 > DistBox2)
			{
				NewLastKnownPoint = UKismetMathLibrary::RandomPointInBoundingBox(PosBox1,Box1->GetScaledBoxExtent()/2);
			}
			else
			{
				NewLastKnownPoint = UKismetMathLibrary::RandomPointInBoundingBox(PosBox2,Box2->GetScaledBoxExtent()/2);
			}
			
			PestilenceAIController->GetBlackboard()->SetValueAsVector(Keys::LastKnownPoint, NewLastKnownPoint);
		}
	}
	return EBTNodeResult::Succeeded;
}
FString UBTTask_PassByHidingSpot::GetStaticDescription() const
{
	return FString("Passes by hiding spot if player is hidden to the furthest pass by area");
}