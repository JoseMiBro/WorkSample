// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/AI/Behaviors/Tasks/BTTask_UpdateFirstTargetPoint.h"

#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Components/AI/PestilenceAIComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

#include <Pestilence/Core/PestilenceGameInstance.h>
#include <Pestilence/Core/PestilenceSaveGameSubsystem.h>
#include <Kismet/GameplayStatics.h>

UBTTask_UpdateFirstTargetPoint::UBTTask_UpdateFirstTargetPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Update First TargetPoint");
}

EBTNodeResult::Type UBTTask_UpdateFirstTargetPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<AAIController> AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	
	APestilenceCharacter* CharOwner = Cast<APestilenceCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	UPestilenceGameInstance* GameInstance = Cast<UPestilenceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	UPestilenceSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
	UPestilenceSaveGame* LoadedSaveGame = nullptr;
	
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;

	if(AIController->GetBlackboardComponent()->GetValueAsBool(Keys::FirstTargetPointUpdate) == true)
		return EBTNodeResult::Succeeded;

	const TObjectPtr<UPestilenceAIComponent> PestilenceAIComponent = Cast<UPestilenceAIComponent>(AIController->GetPawn()->GetComponentByClass(UPestilenceAIComponent::StaticClass()));

	if (!IsValid(PestilenceAIComponent))
		return EBTNodeResult::Failed;

	int32 const TargetPointSize = PestilenceAIComponent->TargetPoints.Num() - 1;

	if (TargetPointSize < 0)
	{
#if WITH_EDITOR
		const FString ActorName = OwnerComp.GetAIOwner()->GetPawn()->GetActorLabel();
		UE_LOG(LogTemp, Warning, TEXT("Advertencia: El actor %s no tiene TargetPoints."), *ActorName);
#endif
		return EBTNodeResult::Failed;
	}
	if (!IsValid(AIController->GetBlackboardComponent()))
		return EBTNodeResult::Failed;

	if (TargetPointIndex > TargetPointSize)
		TargetPointIndex = 0;
	
	ensureMsgf(IsValid(PestilenceAIComponent->TargetPoints[TargetPointIndex]),TEXT("Target Point, is not valid, probably null"));

	if (IsValid(PestilenceAIComponent->TargetPoints[TargetPointIndex]))
	{
		//if (UGameplayStatics::DoesSaveGameExist(SaveSubsystem->GetCurrentSlotName(), 0))
		//{
		//	LoadedSaveGame = Cast<UPestilenceSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSubsystem->GetCurrentSlotName(), 0));
		//	if (LoadedSaveGame)
		//	{
		//		for (FActorSaveData& ActorData : LoadedSaveGame->SavedActors)
		//		{
		//			if (ActorData.CharacterID == CharOwner->ID)
		//			{
		//				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, FString::Printf(TEXT("%s: %d"), *CharOwner->GetName()
		//					, ActorData.SavedTargetPointIndex));

		//				AIController->GetBlackboardComponent()->SetValueAsInt(Keys::TargetPointIndex, ActorData.SavedTargetPointIndex);
		//				return EBTNodeResult::Succeeded;
		//			}
		//		}
		//	}
		//}

		AIController->GetBlackboardComponent()->SetValueAsFloat(Keys::WaitTime, PestilenceAIComponent->TargetPoints[TargetPointIndex]->WaitTime);
		//AIController->GetBlackboardComponent()->SetValueAsVector(Keys::Rotate, PestilenceAIComponent->TargetPoints[TargetPointIndex]->Pivot->GetComponentRotation().Euler());
		AIController->GetBlackboardComponent()->SetValueAsVector(Keys::Rotate, PestilenceAIComponent->TargetPoints[TargetPointIndex]->Pivot->GetComponentLocation());
		AIController->GetBlackboardComponent()->SetValueAsVector(Keys::TargetPointPosition, PestilenceAIComponent->TargetPoints[TargetPointIndex]->GetActorLocation());
		AIController->GetBlackboardComponent()->SetValueAsBool(Keys::FirstTargetPointUpdate, true);

		int NextTargetPointIndex = TargetPointIndex + 1;
		if (NextTargetPointIndex > TargetPointSize)
			NextTargetPointIndex = 0;

		AIController->GetBlackboardComponent()->SetValueAsInt(Keys::TargetPointIndex, NextTargetPointIndex);
	}	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_UpdateFirstTargetPoint::GetStaticDescription() const
{
	return TEXT("Update Only First TargetPoint");
}
