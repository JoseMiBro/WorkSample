// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PostProcessBlendZone.generated.h"

class UBoxComponent;
class USceneComponent;
class UMaterialParameterCollection;

USTRUCT(BlueprintType)
struct FPostProcessBlendParameter
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, Category = "Pestilence | PostProcessBlend")
  FName ParameterName;

  UPROPERTY(EditAnywhere, Category = "Pestilence | PostProcessBlend")
  float StartPointValue = 0.f;
  UPROPERTY(EditAnywhere, Category = "Pestilence | PostProcessBlend")
  float EndPointValue = 0.f;
	
  UPROPERTY(VisibleAnywhere, Category = "Pestilence | PostProcessBlend")
  float CurrentValue = 0.f;
};

UCLASS()
class PESTILENCE_API APostProcessBlendZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APostProcessBlendZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UBoxComponent> TriggerBox;
	
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<USceneComponent> StartPoint;
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<USceneComponent> EndPoint;
	
  UPROPERTY(EditAnywhere, Category = "Pestilence | PostProcessBlend")
  TObjectPtr<UMaterialParameterCollection> ToonShaderParameterCollection;
	
  UPROPERTY(EditAnywhere, Category = "Pestilence | PostProcessBlend")
  TArray<FPostProcessBlendParameter> ParameterArray;
	
private:
	UFUNCTION()
	void OnShapeBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);

	UFUNCTION()
	void OnShapeEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SaveBlendArray();

	FVector StartPos;
	FVector EndPos;
};
