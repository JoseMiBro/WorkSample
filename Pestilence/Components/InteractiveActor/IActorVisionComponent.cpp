#include "Pestilence/Components/InteractiveActor/IActorVisionComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/Characters/PestilenceCharacter.h"


UIActorVisionComponent::UIActorVisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Owner = GetOwner<APestilenceCharacter>();
}

void UIActorVisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult Hit;
	FVector Start = Owner->GetFirstPersonCameraComponent()->GetComponentLocation();

	FVector ForwardVector = Owner->GetFirstPersonCameraComponent()->GetForwardVector();
	FVector End = ((ForwardVector * bLineTraceDistance) + Start);
	FCollisionQueryParams CollisionParams;

	if (Owner->IsPlayerControlled())
	{
		if (bShowDebugLineTrace) { DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1); }

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams))
		{
			if (Hit.bBlockingHit)
			{
				if (Hit.GetActor()->GetClass()->IsChildOf(AInteractiveActor::StaticClass()))
				{
					Owner->CurrentIA = Cast<AInteractiveActor>(Hit.GetActor());
					Owner->CurrentIA->CurrentInteractor = Owner;

					FeedBackText = Owner->CurrentIA->feedBackText;
					feedBackTextDelegate.ExecuteIfBound(FeedBackText);
				}
				UInteractiveBaseComponent* CurrentIC = Hit.GetActor()->FindComponentByClass<UInteractiveBaseComponent>();
				if (CurrentIC)
				{
					// Execute a function from the component
					CurrentInteractiveComp = CurrentIC;
					CurrentInteractiveActor = Hit.GetActor();
				}
			}

		}
		else
		{
		
			// CleanUp
			CurrentInteractiveComp = nullptr;
			CurrentInteractiveActor = nullptr;	

			if (Owner->CurrentIA)
			{
				Owner->CurrentIA->CurrentInteractor = nullptr;
				Owner->CurrentIA = nullptr;
			}

			feedBackTextDelegateExit.ExecuteIfBound();
		}
	}
}

//void UIActorVisionComponent::PerformLineTrace()
//{
//	FHitResult HitResult;
//	FVector StartLocation = Owner->GetFirstPersonCameraComponent()->GetComponentLocation();
//	FVector EndLocation = ((Owner->GetFirstPersonCameraComponent()->GetForwardVector() * bLineTraceDistance) + StartLocation);
//	FCollisionQueryParams CollisionParameters;
//
//	// Line trace
//	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParameters);
//
//	if (bHit)
//	{
//		// Check if the hit actor has the desired component
//		UInteractiveBaseComponent* CurrentIC = HitResult.GetActor()->FindComponentByClass<UInteractiveBaseComponent>();
//		if (CurrentIC)
//		{
//			// Execute a function from the component
//			CurrentInteractiveComp = CurrentIC;
//			CurrentInteractiveActor = HitResult.GetActor();
//		}
//	}
//	else
//	{
//		// CleanUp
//		CurrentInteractiveComp = nullptr;
//		CurrentInteractiveActor = nullptr;
//	}
//}