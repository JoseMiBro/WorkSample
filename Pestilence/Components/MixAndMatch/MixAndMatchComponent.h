// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MixAndMatchComponent.generated.h"

class USkeletalMesh;
class UMaterialInterface;
class UArchetypeComponent;
class UAnimInstance;

USTRUCT(BlueprintType)
struct FMeshAndMaterialStruct
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mix & Match")
	TObjectPtr<USkeletalMesh> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mix & Match")
	TObjectPtr<UMaterialInterface> Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mix & Match")
	TObjectPtr<UMaterialInterface> SecondMaterial;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UMixAndMatchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMixAndMatchComponent();

	UFUNCTION(CallInEditor,BlueprintCallable)
		void GenerateCharacter();
	UFUNCTION()
		virtual void BeginPlay() override;
	void RandomizeCharacter();

	void RandomizePiece(USkeletalMeshComponent* PieceSlot, TArray<FMeshAndMaterialStruct>& PieceMap);
	void SaveMeshesAndMaterial();

	void CreateMesh();

	UPROPERTY(EditAnywhere, Category ="Mix & Match")
		bool RandomGeneration = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mix & Match")
		TArray<FMeshAndMaterialStruct>HeadMeshAndMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mix & Match")
		TArray<FMeshAndMaterialStruct>ChestMeshAndMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mix & Match")
		TArray<FMeshAndMaterialStruct>LegsMeshAndMaterial;

	UPROPERTY()
		TObjectPtr<USkeletalMesh> HeadMeshSave;
	UPROPERTY()
		TObjectPtr<UMaterialInterface> HeadMatSave;
	UPROPERTY()
		TObjectPtr<USkeletalMesh> ChestMeshSave;
	UPROPERTY()
		TObjectPtr<UMaterialInterface> ChestMatSave;
	UPROPERTY()
		TObjectPtr<USkeletalMesh> LegsMeshSave;
	UPROPERTY()
		TObjectPtr<UMaterialInterface> LegsMatSave;
};
