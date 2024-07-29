// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CustomPlaySound.h"

#include "Kismet/GameplayStatics.h"
#include "Pestilence/AI/PestilenceAIController.h"

EBTNodeResult::Type UBTTask_CustomPlaySound::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<APestilenceAIController> Controller = Cast<APestilenceAIController>(OwnerComp.GetAIOwner());
	
	if (IsValid(Controller) &&
		(IsValid(Controller->SoundToPlayOnInfection) || IsValid(Controller->SoundToPlayOnLostInterest)))
	{
		if(Sound == ESoundToPlay::InfectionSound)
		{
			Controller->OnLostInterest.Broadcast(SoundType::Infection);
			return EBTNodeResult::Succeeded; 
		}
		Controller->OnLostInterest.Broadcast(SoundType::BottleBraking);
		return EBTNodeResult::Succeeded; 
	}
	return EBTNodeResult::Failed;
}
