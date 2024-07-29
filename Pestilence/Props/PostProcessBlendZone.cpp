// Fill out your copyright notice in the Description page of Project Settings.


#include "PostProcessBlendZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pestilence/Core/PestilenceSaveGameSubsystem.h"
#include "Pestilence/Core/PestilenceSaveGame.h"
#include "../Core/PestilenceGameInstance.h"

// Sets default values
APostProcessBlendZone::APostProcessBlendZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
  RootComponent = TriggerBox;

  StartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Start Point"));
  StartPoint->SetupAttachment(TriggerBox);
  EndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("End Point"));
  EndPoint->SetupAttachment(TriggerBox);
}

// Called when the game starts or when spawned
void APostProcessBlendZone::BeginPlay()
{
  Super::BeginPlay();

  SetActorTickEnabled(false);

  TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APostProcessBlendZone::OnShapeBeginOverlapCallback);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APostProcessBlendZone::OnShapeEndOverlapCallback);

  StartPos = StartPoint->GetRelativeLocation();
  StartPos.Y = 0;
  StartPos.Z = 0;
  EndPos = EndPoint->GetRelativeLocation();
  EndPos.Z = 0;
}

// Called every frame
void APostProcessBlendZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
  FVector LocalPlayerPos = UKismetMathLibrary::InverseTransformLocation(GetTransform(), Player->GetActorLocation());
  LocalPlayerPos.Y = 0;
  LocalPlayerPos.Z = 0;

  float SqrDistanceStart = StartPos.X < LocalPlayerPos.X ? FVector::DistSquared(StartPos, LocalPlayerPos) : 0;
  float SqrDistanceEnd = LocalPlayerPos.X < EndPos.X ? FVector::DistSquared(EndPos, LocalPlayerPos) : 0;

  float EndValue = SqrDistanceEnd / (SqrDistanceStart + SqrDistanceEnd);

  UMaterialParameterCollectionInstance* ToonShaderParameterInstance = GetWorld()->GetParameterCollectionInstance(ToonShaderParameterCollection);
  for(int i = 0; i < ParameterArray.Num(); ++i)
  {
    ParameterArray[i].CurrentValue = FMath::Lerp(ParameterArray[i].EndPointValue, ParameterArray[i].StartPointValue, EndValue);
    ToonShaderParameterInstance->SetScalarParameterValue(ParameterArray[i].ParameterName, ParameterArray[i].CurrentValue);
  }
}

void APostProcessBlendZone::OnShapeBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
  const FHitResult& SweepResult)
{
  APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
  if (OtherActor == Player)
    SetActorTickEnabled(true);
}

void APostProcessBlendZone::OnShapeEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  SaveBlendArray();

  APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
  if (OtherActor == Player)
    SetActorTickEnabled(false);
}

void APostProcessBlendZone::SaveBlendArray()
{
  UPestilenceGameInstance* GameInstance = Cast<UPestilenceGameInstance>(GetGameInstance());
  GameInstance->ParameterArray = ParameterArray;
}
