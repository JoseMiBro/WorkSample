#include "RagdollState.h"
#include "PestilencePossessionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/WidgetComponent.h"

void URagdollState::CreateState()
{ 

}

void URagdollState::EnterState()
{
  const auto Possession = GetPossessionComponent();

  if (!Possession) return;

  Owner = Possession->GetOwner<APestilenceCharacter>();
  TArray<UMeshComponent*> Components;
  Owner->GetComponents<UMeshComponent>(Components);
  if (Components.Num() > 0)
  {
    for (int i = 0; i < Components.Num(); ++i)
    {
      if (!Components[i]->IsVisible() || Components[i]->bOnlyOwnerSee || Components[i]->IsA(UWidgetComponent::StaticClass())) continue;

      for(int j = 0; j < Components[i]->GetMaterials().Num(); ++j)
      {
        UMaterialInstanceDynamic* DynamicDissolveMaterial = UMaterialInstanceDynamic::Create(Components[i]->GetMaterials()[j], this);
        DynamicDissolveMaterials.Push(DynamicDissolveMaterial);
        Components[i]->SetMaterial(j, DynamicDissolveMaterial);
        DynamicDissolveMaterial->SetScalarParameterValue(GetPossessionComponent()->DissolveControlName, 1);
        DynamicDissolveMaterial->SetScalarParameterValue(GetPossessionComponent()->DissolveOffsetName, -1);
      }
    }
  }
  /*
  if (DissolveMaterials.Num() > 0)
  {
    for (int i = 0; i < DissolveMaterials.Num(); ++i)
    {
      UMaterialInstanceDynamic* DynamicDissolveMaterial = UMaterialInstanceDynamic::Create(DissolveMaterials[i], this);
      DynamicDissolveMaterials.Push(DynamicDissolveMaterial);
    }
  }else
  {
    UE_LOG(LogTemp, Warning, TEXT("Mesh has no materials"));
	}
  */

	UCapsuleComponent* CapsuleComp = Owner->GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	Owner->GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	Owner->GetMesh()->SetSimulatePhysics(true);
	Owner->SetActorEnableCollision(true);
	
	if (DynamicDissolveMaterials.Num() > 0)
  {
		UGameplayStatics::PlaySoundAtLocation(GetPossessionComponent(), GetPossessionComponent()->DissolveSound, GetPossessionComponent()->GetOwner()->GetActorLocation());
	}
}

void URagdollState::UpdateState(float DeltaTime)
{
	if (DynamicDissolveMaterials.Num() > 0 && DissolveLevel <= 1.f)
	{
    	DissolveLevel += GetPossessionComponent()->DissolveSpeed * DeltaTime;
    		for (int i = 0; i < DynamicDissolveMaterials.Num(); ++i)
    		{
    		  DynamicDissolveMaterials[i]->SetScalarParameterValue(GetPossessionComponent()->DissolveOffsetName, DissolveLevel);
    		}
	}
	else if (!bIsDestroyed)
	{
		TObjectPtr<APestilenceAIController> Controller = Cast<APestilenceAIController>(Owner->GetControllerAI());
		if(IsValid(Controller))
		{
			bIsDestroyed = IsValid(Controller);
			Controller->GetBlackboard()->SetValueAsEnum(Keys::State,static_cast<uint8>(EPestilenceStates::Dead));
			Controller->SetCurrentState(EPestilenceStates::Dead);
		}
		Owner->OnKill();
	}
	
}