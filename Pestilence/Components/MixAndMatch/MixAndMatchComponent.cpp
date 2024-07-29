// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Components/MixAndMatch/MixAndMatchComponent.h"
#include "Pestilence/Components/MixAndMatch/MixAndMatchSkeletalMeshComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/Possession/ArchetypeComponent.h"
#include "Pestilence/Enums/Archetype.h"
#include "SkeletalMergingLibrary.h"

// Sets default values for this component's properties
UMixAndMatchComponent::UMixAndMatchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


void UMixAndMatchComponent::GenerateCharacter()
{
	if (!IsValid(GetOwner())) return; 
	const TObjectPtr<APestilenceCharacter> pCharacter = Cast<APestilenceCharacter>(GetOwner());
	if (!IsValid(pCharacter)) return; 
	const TObjectPtr<UArchetypeComponent> pComponent = pCharacter->GetArchetypeComponent();
	if (!IsValid(pComponent)) return;

	const EArchetype Archetype = pComponent->GetPestilenceArchetype();
	
	RandomizeCharacter();
	SaveConfig();
}

void UMixAndMatchComponent::BeginPlay()
{
	Super::BeginPlay();
	if(RandomGeneration)
		GenerateCharacter();

	CreateMesh();
}

void UMixAndMatchComponent::RandomizeCharacter()
{
	if (!IsValid(GetOwner())) return; 
	const TObjectPtr<APestilenceCharacter> pCharacter = Cast<APestilenceCharacter>(GetOwner());
	if (!IsValid(pCharacter)) return;

	int32 seed = FMath::Rand() + 5;
	FMath::RandInit(seed);

  pCharacter->SkeletalMeshMergeParams.MeshesToMerge.Empty();

	if (IsValid(pCharacter->GetHead()))
		RandomizePiece(pCharacter->GetHead(),HeadMeshAndMaterial);
	if (IsValid(pCharacter->GetChest())) 
		RandomizePiece(pCharacter->GetChest(),ChestMeshAndMaterial);
	if (IsValid(pCharacter->GetLegs()))
		RandomizePiece(pCharacter->GetLegs(),LegsMeshAndMaterial);

  //SaveMeshesAndMaterial(); 

}

void UMixAndMatchComponent::RandomizePiece(USkeletalMeshComponent* PieceSlot,TArray<FMeshAndMaterialStruct>& PieceMap)
{
	if (PieceMap.IsEmpty())
	{
		PieceSlot->SetSkeletalMesh(nullptr);
		PieceSlot->SetMaterial(0,nullptr);
		PieceSlot->SetMaterial(1,nullptr);
	}
	else
	{
		int32 RandomInt = FMath::RandRange(0, PieceMap.Num() - 1);

		PieceSlot->SetSkeletalMesh(PieceMap[RandomInt].Mesh);
		PieceSlot->SetMaterial(0, PieceMap[RandomInt].Material);
		if(PieceMap[RandomInt].SecondMaterial != nullptr)
			PieceSlot->SetMaterial(1, PieceMap[RandomInt].SecondMaterial);
		

    const TObjectPtr<APestilenceCharacter> pCharacter = Cast<APestilenceCharacter>(GetOwner());
    if (!IsValid(pCharacter)) return;
    pCharacter->SkeletalMeshMergeParams.MeshesToMerge.Push(PieceMap[RandomInt].Mesh);
	}
}

void UMixAndMatchComponent::SaveMeshesAndMaterial()
{
	if (!IsValid(GetOwner())) return; 
	const TObjectPtr<APestilenceCharacter> pCharacter = Cast<APestilenceCharacter>(GetOwner());
	if (!IsValid(pCharacter)) return;

	if (IsValid(pCharacter->GetHead())) 
	{
		HeadMeshSave = pCharacter->GetHead()->GetSkeletalMeshAsset();
		HeadMatSave = pCharacter->GetHead()->GetMaterial(0);
	}

	if (IsValid(pCharacter->GetChest()))
	{
		ChestMeshSave = pCharacter->GetChest()->GetSkeletalMeshAsset();
		ChestMatSave = pCharacter->GetChest()->GetMaterial(0);
  }
	
	if (IsValid(pCharacter->GetLegs()))
	{
		LegsMeshSave = pCharacter->GetLegs()->GetSkeletalMeshAsset();
		LegsMatSave = pCharacter->GetLegs()->GetMaterial(0);
  }
	SaveConfig();
}

void UMixAndMatchComponent::CreateMesh()
{
  const TObjectPtr<APestilenceCharacter> Character = Cast<APestilenceCharacter>(GetOwner());
  if (!IsValid(Character)) return;
  USkeletalMesh* MixAndMatchMesh = USkeletalMergingLibrary::MergeMeshes(Character->SkeletalMeshMergeParams);
  if (MixAndMatchMesh)
  {
    Character->GetMesh()->SetSkeletalMesh(MixAndMatchMesh);
    unsigned int NumMaterials = 0;
    if (Character->GetHead()->GetSkeletalMeshAsset())
    {
      const auto HeadMaterials = Character->GetHead()->GetMaterials();
      for (int i = 0; i < HeadMaterials.Num(); ++i)
      {
        if (!HeadMaterials[i]) continue;
        Character->GetMesh()->SetMaterial(NumMaterials, HeadMaterials[i]);
        ++NumMaterials;
      }
    }
    if (Character->GetChest()->GetSkeletalMeshAsset())
    {
      const auto ChestMaterials = Character->GetChest()->GetMaterials();
      for (int i = 0; i < ChestMaterials.Num(); ++i)
      {
        if (!ChestMaterials[i]) continue;
        Character->GetMesh()->SetMaterial(NumMaterials, ChestMaterials[i]);
        ++NumMaterials;
      }
    }
    if (Character->GetLegs()->GetSkeletalMeshAsset())
    {
      const auto LegsMaterials = Character->GetLegs()->GetMaterials();
      for (int i = 0; i < LegsMaterials.Num(); ++i)
      {
        if (!LegsMaterials[i]) continue;
        Character->GetMesh()->SetMaterial(NumMaterials, LegsMaterials[i]);
        ++NumMaterials;
      }
    }
  }
}
