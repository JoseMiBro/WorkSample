#include "PestilenceCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NPCManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/Components/Abilities/PestilenceAbilityComponent.h"
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Pestilence/Components/Possession/PestilenceVisionComponent.h"
#include "Pestilence/Components/InteractiveActor/InteractiveBaseComponent.h"
#include "Pestilence/InteractiveActors/InteractiveActor.h"
#include "Pestilence/InteractiveActors/IActors/PickUpBase.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Pestilence/Components/Perception/SuspiciousActionSenderComponent.h"
#include "Pestilence/UI/PlayerHUDWidget.h"
#include "Pestilence/Components/HUD/PestilencePauseComponent.h"
#include "Pestilence/Components/Interaction/InteractableComponent.h"
#include "Pestilence/Components/Interaction/InteractorComponent.h"
#include "Pestilence/Components/Interaction/InventoryComponent.h"
#include "Pestilence/Components/Interaction/PickableReceiverComponent.h"
#include "Pestilence/Components/Possession/ArchetypeComponent.h"
#include "Pestilence/Components/TakeDown/TakeDownComponent.h"
#include "Pestilence/Components/MixAndMatch/MixAndMatchComponent.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "../Components/Abilities/ProjectilePredictionVision.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pestilence/Components/AI/PestilenceAIComponent.h"
#include "Pestilence/Components/MixAndMatch/MixAndMatchSkeletalMeshComponent.h"
#include "Pestilence/Components/Perception/PestilenceHearingComponent.h"
#include "Pestilence/Core/PestilenceSaveGameSubsystem.h"
#include "Pestilence/Core/PestilenceGameInstance.h"
#include "../Components/FailState/FailStateComponent.h"
#include "../Core/PestilenceGameMode.h"

APestilenceCharacter::APestilenceCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPestilenceMovementComponent>(TEXT("CharMoveComp")))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	CameraPivotComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPivotComponent"));
	CameraPivotComponent->SetupAttachment(RootComponent);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(CameraPivotComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;


	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

  AbilityAttachment = CreateDefaultSubobject<USceneComponent>(TEXT("AbilityAttachment"));
  AbilityAttachment->SetupAttachment(Mesh1P, TEXT("AbilitySocket"));

	//Create meshes to MixAndMatch
	//MixAndMatchRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MixMatchRoot"));
	//MixAndMatchRootComponent->SetupAttachment(RootComponent);
	//MixAndMatchRootComponent->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FQuat(0, 0, -90, 0));

	Head = CreateDefaultSubobject<UMixAndMatchSkeletalMeshComponent>(TEXT("Head Skeletal Mesh Component"));
	Head->bOwnerNoSee = true;

	Head->SetupAttachment(RootComponent);

	Chest = CreateDefaultSubobject<UMixAndMatchSkeletalMeshComponent>(TEXT("Chest Skeletal Mesh Component"));
	Chest->bOwnerNoSee = true;

	Chest->SetupAttachment(RootComponent);

	Legs = CreateDefaultSubobject<UMixAndMatchSkeletalMeshComponent>(TEXT("Legs Skeletal Mesh Component"));
	Legs->bOwnerNoSee = true;

	Legs->SetupAttachment(RootComponent);

	PestilencePossessionComponent = CreateDefaultSubobject<UPestilencePossessionComponent>(TEXT("Possession"));
	PestilenceVisionComponent = CreateDefaultSubobject<UPestilenceVisionComponent>(TEXT("Vision"));

	NPCBaseComponent = CreateDefaultSubobject<UInteractiveBaseComponent>(TEXT("NPCBaseComponent"));
	CurrentIA = nullptr;
	CurrentPickUp = nullptr;

	// Create a PickUp Attachment
	IAHolding = CreateDefaultSubobject<USceneComponent>(TEXT("IActorHoldingComponent"));
	IAHolding->SetupAttachment(FirstPersonCameraComponent);
	IAHolding->SetRelativeLocation(FVector(30.f, -47.f, -25.f));

	//Pause
	PauseComponent = CreateDefaultSubobject<UPestilencePauseComponent>(TEXT("Pause"));

	PestilenceMovementComponent = Cast<UPestilenceMovementComponent>(GetCharacterMovement());

	SuspiciousActionSenderComponent = CreateDefaultSubobject<USuspiciousActionSenderComponent>
		(TEXT("SuspiciousActionSender"));

  PestilenceAbilityComponent = CreateDefaultSubobject<UPestilenceAbilityComponent>
    (TEXT("PestilenceAbilityComponent"));
    (TEXT("PestilenceAbilityComponent"));

	ArchetypeComponent = CreateDefaultSubobject<UArchetypeComponent>(TEXT("Archetype"));

	DissolveMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("Dissolve"));

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Sphere"));
	InteractionSphere->SetupAttachment(FirstPersonCameraComponent);
	InteractionSphere->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
	InteractionSphere->SetSphereRadius(125.f);
	
	InteractorComponent = CreateDefaultSubobject<UInteractorComponent>(TEXT("Interactor"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	InventoryAttachment = CreateDefaultSubobject<USceneComponent>(TEXT("Inventory Attachment"));
	
	InventoryAttachment->SetupAttachment(FirstPersonCameraComponent);

	// Required components for receive interactions
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	InteractableCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractableCapsule"));
	InteractableCapsuleComponent->SetupAttachment(RootComponent);

	TakeDownComponent = CreateDefaultSubobject<UTakeDownComponent>(TEXT("TakeDown"));
	FailStateComponent = CreateDefaultSubobject <UFailStateComponent>(TEXT("FailState"));

	MixAndMatchComponent = CreateDefaultSubobject<UMixAndMatchComponent>(TEXT("MixAndMatchComponent"));

  InfectParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraParticles"));
  InfectParticles->SetupAttachment(GetMesh(), TEXT("Head"));
  ImpactParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Impact Particles"));
  ImpactParticles->SetupAttachment(GetMesh());

  LeftTakeDownParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Left TakeDown Particles"));
	LeftTakeDownParticles->SetupAttachment(Mesh1P, TEXT("TakedownSocket_l"));
	RightTakeDownParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Right TakeDown Particles"));
	RightTakeDownParticles->SetupAttachment(Mesh1P, TEXT("TakedownSocket_r"));

	ProjectilePredictionVision = CreateDefaultSubobject<UProjectilePredictionVision>(TEXT("Projectile Prediction Vision"));
}

void APestilenceCharacter::BeginPlay()
{
	Super::BeginPlay();

	CheckSavedState();

	ChangeMappingContext(DefaultMappingContext);
	AddMappingContext(KeyboardControllerMappingContext, 1);

	if (PlayerHUDClass)
	{
		//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//PlayerHUD = CreateWidget<UPlayerHUDWidget>(PlayerController, PlayerHUDClass);
		//PlayerHUD->AddToPlayerScreen();
	}
	
	if (InteractorComponent)
	{
		InteractorComponent->Initialize(InteractionSphere, GetFirstPersonCameraComponent());
		if (!PestilencePossessionComponent->IsPossessed())
			InteractorComponent->SetActive(false);
	}
	if (InventoryComponent)
		InventoryComponent->Initialize(InventoryAttachment);

	if (InteractableComponent && InteractableCapsuleComponent)
		InteractableComponent->Initialize(InteractableCapsuleComponent);

	if (PestilencePossessionComponent)
	{
		PestilencePossessionComponent->OnEndPossessEvent().AddDynamic(this, &APestilenceCharacter::OnEndPossessCallback);
		PestilencePossessionComponent->OnPossessed.AddDynamic(this, &APestilenceCharacter::OnPossessed);
	}
	PestilenceAIController = Cast<APestilenceAIController>(Controller);


	AIComponent = FindComponentByClass<UPestilenceAIComponent>();

  AbilityAttachment->AttachToComponent(Mesh1P, {
    EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true
    }, TEXT("AbilitySocket"));

  OriginalLocation = GetActorLocation();

  APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
  Mesh1P->AttachToComponent(PlayerCameraManager->GetTransformComponent(), {
    EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true
    });

	TObjectPtr<APestilenceGameMode> GameMode = Cast<APestilenceGameMode>(GetWorld()->GetAuthGameMode());
	ManagerReference = GameMode->GetNPCManager();
	ManagerReference->AddNPC(this);

	// bUseControllerRotationYaw = false;
	// GetPestilenceMovementComponent()->bOrientRotationToMovement = true;
	// if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == this)
	// {
	// 	bUseControllerRotationYaw = 1;
	// 	GetPestilenceMovementComponent()->bOrientRotationToMovement = false;
	// }
}

void APestilenceCharacter::BeginDestroy()
{
	if(IsValid(ManagerReference))
		ManagerReference->RemoveNPC(this);
	Super::BeginDestroy();
}

void APestilenceCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::CrouchInput);

		// Walk
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::TryWalk);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::Look);
		EnhancedInputComponent->BindAction(LookGamepadAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::Look);

		// Leaning
		EnhancedInputComponent->BindAction(LeanLeftAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::LeanLeft);
		EnhancedInputComponent->BindAction(LeanRightAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::LeanRight);
		
		// Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::Roll);

		// Pestilence Vision
		EnhancedInputComponent->BindAction(PestilenceVisionAction, ETriggerEvent::Started, this, &APestilenceCharacter::StartVision);
		EnhancedInputComponent->BindAction(PestilenceVisionAction, ETriggerEvent::Completed, this, &APestilenceCharacter::CancelVision);

		// Pestilence Possession
		EnhancedInputComponent->BindAction(PestilencePossessionAction, ETriggerEvent::Started, this, &APestilenceCharacter::StartPossession);
		EnhancedInputComponent->BindAction(PestilencePossessionAction, ETriggerEvent::Completed, this, &APestilenceCharacter::CancelPossession);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APestilenceCharacter::Interact);

		//Interact
		EnhancedInputComponent->BindAction(LaunchAction, ETriggerEvent::Started, this, &APestilenceCharacter::Launch);
		
		// Ability
		EnhancedInputComponent->BindAction(CastAbilityAction, ETriggerEvent::Started, this, &APestilenceCharacter::StartCast);
		EnhancedInputComponent->BindAction(CastAbilityAction, ETriggerEvent::Completed, this, &APestilenceCharacter::CancelCast);
		EnhancedInputComponent->BindAction(PrepareAbilityAction, ETriggerEvent::Started, this, &APestilenceCharacter::StartPrepare);
		EnhancedInputComponent->BindAction(PrepareAbilityAction, ETriggerEvent::Completed, this, &APestilenceCharacter::CancelPrepare);
		EnhancedInputComponent->BindAction(SelectAbility1Action, ETriggerEvent::Started, this, &APestilenceCharacter::SelectAbility1);
		EnhancedInputComponent->BindAction(SelectAbility2Action, ETriggerEvent::Started, this, &APestilenceCharacter::SelectAbility2);
		EnhancedInputComponent->BindAction(SelectAbility3Action, ETriggerEvent::Started, this, &APestilenceCharacter::SelectAbility3);
		EnhancedInputComponent->BindAction(SelectAbility4Action, ETriggerEvent::Started, this, &APestilenceCharacter::SelectAbility4);
		EnhancedInputComponent->BindAction(SelectAbilityNextAction, ETriggerEvent::Started, this, &APestilenceCharacter::SelectAbilityNext);
		EnhancedInputComponent->BindAction(SelectAbilityPrevAction, ETriggerEvent::Started, this, &APestilenceCharacter::SelectAbilityPrev);
		
		//Pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &APestilenceCharacter::AddPauseMenu);
		EnhancedInputComponent->BindAction(DiaryAction, ETriggerEvent::Started, this, &APestilenceCharacter::AddDiaryMenu);

		EnhancedInputComponent->BindAction(SaveAction, ETriggerEvent::Started, this, &APestilenceCharacter::QuickSaveAction);
		EnhancedInputComponent->BindAction(LoadAction, ETriggerEvent::Started, this, &APestilenceCharacter::QuickLoadAction);

		EnhancedInputComponent->BindAction(KeyboardAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::SetKeyboardControl);
		EnhancedInputComponent->BindAction(ControllerAction, ETriggerEvent::Triggered, this, &APestilenceCharacter::SetControllerControl);
		
		//DEBUG FOR NOW SO THAT THE HEARING EFFECT OF THE AI CAN BE TRIED
		// hijo mio como que can be tried para eso me escribes en español
		//EnhancedInputComponent->BindAction(PestilenceGenerateSoundAction, ETriggerEvent::Started, this, &APestilenceCharacter::GenerateSound);
	}
}

//Movement Input Callbacks

void APestilenceCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (!Controller) return;

	PestilenceMovementComponent->Move(MovementVector);

	if(!MovementVector.IsNearlyZero())
		GenerateSound();
}

void APestilenceCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (!Controller) return;

	//LookAxisVector = LookAxisVector * UGameplayStatics::GetWorldDeltaSeconds(this);
	LookAxisVector.X = bUsingController ? LookAxisVector.X * ControllerSensitivityX : LookAxisVector.X * MouseSensitivityX;
	LookAxisVector.Y = bUsingController ? LookAxisVector.Y * ControllerSensitivityY : LookAxisVector.Y * MouseSensitivityY;
	LookAxisVector = bInMinigame ? LookAxisVector * LockpickCameraMultiplier : LookAxisVector;
	PestilenceMovementComponent->Look(LookAxisVector);
}

void APestilenceCharacter::LeanLeft(const FInputActionValue& Value)
{
	float LeanAmount = Value.Get<float>();
	if (!Controller) return;
	if (LeanAmount == 0 && PestilenceMovementComponent->GetTargetLeanAmount() > 0 || LastLeanInput == LeanAmount) return;

	LeanAmount = LeanAmount > -TriggerThreshold ? 0 : LeanAmount;
	LastLeanInput = LeanAmount;

	PestilenceMovementComponent->TryLean(LeanAmount);
}

void APestilenceCharacter::LeanRight(const FInputActionValue& Value)
{
	float LeanAmount = Value.Get<float>();
	if (!Controller) return;
	if (LeanAmount == 0 && PestilenceMovementComponent->GetTargetLeanAmount() < 0 || LastLeanInput == LeanAmount) return;

	LeanAmount = LeanAmount < TriggerThreshold ? 0 : LeanAmount;
	LastLeanInput = LeanAmount;

	PestilenceMovementComponent->TryLean(LeanAmount);
}

void APestilenceCharacter::Interact()
{
	bool bAlreadyInteracted = false;
	
	if (InteractorComponent && InteractorComponent->IsLookingAnInteractable())
	{
		const auto* InteractableOwner = InteractorComponent->GetLookingInteractable()->GetOwner();
		if (!InteractableOwner || !InteractableOwner->IsValidLowLevel()) return;

		if (const auto* BasePickableReceiver = InteractableOwner
			->GetComponentByClass(UPickableReceiverComponent::StaticClass()))
		{
			// Give the current interactable
			InteractorComponent->TryInteract();
			bAlreadyInteracted = true;
		}
	}

	if (InventoryComponent && InventoryComponent->HasPickable() && !bAlreadyInteracted)
	{
		InventoryComponent->DropPickable();
		bAlreadyInteracted = true;
	}

	if (InteractorComponent && InteractorComponent->IsLookingAnInteractable() && !bAlreadyInteracted)
	{
		InteractorComponent->TryInteract();
		bAlreadyInteracted = true;
	}

	if (CurrentIA)
	{
		if (!CurrentIA->GetClass()->IsChildOf(APickUpBase::StaticClass()))
		{
			CurrentIA->DefaultInteraction(this);
		}
		else if (!CurrentPickUp)
		{
			CurrentIA->DefaultInteraction(this);
		}
	}
	else if (CurrentPickUp != nullptr)
	{
		CurrentPickUp->DefaultInteraction(this);
	}
}

void APestilenceCharacter::AIInteract()
{
	TArray<AActor*> OverlappingActors;

	if(AIComponent->SphereComponent)
	{
		AIComponent->SphereComponent->GetOverlappingActors(OverlappingActors, AActor::StaticClass());
	}

	for (const AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor != this)
		{
			if(const TObjectPtr<UInteractableComponent> Interactable = OverlappingActor->FindComponentByClass<UInteractableComponent>())
			{
				if (Interactable->InteractableType == EInteractableType::Climb || Interactable->InteractableType == EInteractableType::Door)
				{
					FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), OverlappingActor->GetActorLocation());

					NewRotation.Pitch = 0.f;
					NewRotation.Roll = 0.f;
					
					SetActorRotation(NewRotation);
					
					Interactable->Interact(this->InteractorComponent);

					break;
				}
			}
		}
	}
}

void APestilenceCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Destroyed || EndPlayReason == EEndPlayReason::RemovedFromWorld)
	{
		UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
		UPestilenceSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
		AAIController* AIController =GetControllerAI();

		UPestilenceSaveGame* LoadedSaveGame = Cast<UPestilenceSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSubsystem->GetCurrentSlotName(), 0));

		if (LoadedSaveGame)
		{
			bool bFoundExistingActor = false;

			for (FActorSaveData& ActorData : LoadedSaveGame->SavedActors)
			{
				if (ActorData.CharacterID == ID)
				{
					ActorData.Transform = this->GetActorTransform();
					ActorData.bIsDead = bIsDead;
					bFoundExistingActor = true;

					//if (AIController)
					//{
					//	ActorData.SavedTargetPointIndex = AIController->GetBlackboardComponent()->GetValueAsInt(Keys::TargetPointIndex);
					//}

					break;
				}
			}
			if (!bFoundExistingActor)
			{
				FActorSaveData ActorData;
				ActorData.CharacterID = ID;
				ActorData.Transform = this->GetActorTransform();
				ActorData.bIsDead = bIsDead;

				//if (AIController)
				//{
				//	ActorData.SavedTargetPointIndex = AIController->GetBlackboardComponent()->GetValueAsInt(Keys::TargetPointIndex);
				//}

				LoadedSaveGame->SavedActors.Add(ActorData);

			}

			UGameplayStatics::SaveGameToSlot(LoadedSaveGame, SaveSubsystem->GetCurrentSlotName(), 0);
		}
	}
}

void APestilenceCharacter::CheckSavedState()
{
	UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
	UPestilenceSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
	UPestilenceSaveGame* LoadedSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSubsystem->GetCurrentSlotName(), 0))
	{
		LoadedSaveGame = Cast<UPestilenceSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSubsystem->GetCurrentSlotName(), 0));
	}

	if (LoadedSaveGame)
	{
		for (FActorSaveData& ActorData : LoadedSaveGame->SavedActors)
		{
			if (ActorData.CharacterID == ID)
			{
				if (ActorData.bIsDead && GameInstance->bLoadGame) 
				{
					OnKill();
					return;
				}
				//else if (this != GetWorld()->GetFirstPlayerController()->GetPawn())
				//{
				//	FVector LocationDifference = GetTransform().GetLocation() - ActorData.Transform.GetLocation();

				//	if (LocationDifference.Size() > 100.0f)
				//	{
				//		SetActorTransform(ActorData.Transform);
				//	}
				//}
			}
		}
	}
}

void APestilenceCharacter::Launch()
{
	if (InventoryComponent && InventoryComponent->HasPickable())
	{
		InventoryComponent->DropPickable(true);
	}
}

void APestilenceCharacter::StartCast()
{
  if (PestilenceAbilityComponent)
    PestilenceAbilityComponent->StartCast();
}
void APestilenceCharacter::CancelCast()
{
  if (PestilenceAbilityComponent)
    PestilenceAbilityComponent->CancelCast();
}

void APestilenceCharacter::StartPrepare()
{
  if (PestilenceAbilityComponent)
    PestilenceAbilityComponent->StartPrepare();
}

void APestilenceCharacter::CancelPrepare()
{
  if (PestilenceAbilityComponent)
    PestilenceAbilityComponent->CancelPrepare();
}

void APestilenceCharacter::SelectAbility1()
{
	if (PestilenceAbilityComponent)
		PestilenceAbilityComponent->SelectAbility(0);
}

void APestilenceCharacter::SelectAbility2()
{
	if (PestilenceAbilityComponent)
		PestilenceAbilityComponent->SelectAbility(1);
}

void APestilenceCharacter::SelectAbility3()
{
	if (PestilenceAbilityComponent)
		PestilenceAbilityComponent->SelectAbility(2);
}

void APestilenceCharacter::SelectAbility4()
{
	if (PestilenceAbilityComponent)
		PestilenceAbilityComponent->SelectAbility(3);
}

void APestilenceCharacter::SelectAbilityNext()
{
	if (PestilenceAbilityComponent)
		PestilenceAbilityComponent->SelectAbilityNext();
}

void APestilenceCharacter::SelectAbilityPrev()
{
	if (PestilenceAbilityComponent)
		PestilenceAbilityComponent->SelectAbilityPrev();
}

void APestilenceCharacter::AddPauseMenu() 
{
	if(PauseComponent)
	{
		PauseComponent->AddPauseMenu();
	}
}

void APestilenceCharacter::AddDiaryMenu()
{
	if (PauseComponent) 
	{
		PauseComponent->AddDiaryMenu();
	}
}

void APestilenceCharacter::SetKeyboardControl()
{
	if (!bUsingController) return;

  bUsingController = false;
  APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
	GameMode->SetKeyboardControl();
}

void APestilenceCharacter::SetControllerControl()
{
  if (bUsingController) return;

  bUsingController = true;
  APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
  GameMode->SetControllerControl();
}

void APestilenceCharacter::ChangeToLockpinkMappingContext()
{
  ChangeMappingContext(LockpickingMappingContext);
}

void APestilenceCharacter::ChangeToDefaultMappingContext()
{
	ChangeMappingContext(DefaultMappingContext);
}


void APestilenceCharacter::ChangeMappingContext(UInputMappingContext* _NewMappingContext)
{
  if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
  {
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
      Subsystem->RemoveMappingContext(CurrentMappingContext);
      Subsystem->AddMappingContext(_NewMappingContext, 0);
			CurrentMappingContext = _NewMappingContext;
			bInMinigame = CurrentMappingContext == LockpickingMappingContext;
    }
  }
}

void APestilenceCharacter::AddMappingContext(UInputMappingContext* _NewMappingContext, int _Priority)
{
  if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
  {
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
      Subsystem->AddMappingContext(_NewMappingContext, _Priority);
    }
  }
}

void APestilenceCharacter::RemoveMappingContext(UInputMappingContext* _MappingContextToRemove)
{
  if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
  {
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
      Subsystem->RemoveMappingContext(_MappingContextToRemove);
    }
  }
}

void APestilenceCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

APestilenceAIController* APestilenceCharacter::GetControllerAI() const { return PestilenceAIController; }

void APestilenceCharacter::CrouchInput() {	PestilenceMovementComponent->TryCrouch(); }

void APestilenceCharacter::TryWalk(const FInputActionValue& Value)
{
  bool bTryWalk = Value.Get<bool>();
  PestilenceMovementComponent->TryWalk(bTryWalk);
}

void APestilenceCharacter::TryWalkToggle()
{
  PestilenceMovementComponent->TryWalkToggle();
}

void APestilenceCharacter::Roll() { PestilenceMovementComponent->StartRoll(Internal_GetLastMovementInputVector()); }

void APestilenceCharacter::GenerateSound()
{
	if(PestilenceMovementComponent)
	{
		float MaxRange = 0;

		if(PestilenceMovementComponent->IsCrouching() || PestilenceMovementComponent->Velocity.SizeSquared() < PestilenceMovementComponent->GetMediumSpeed() * PestilenceMovementComponent->GetMediumSpeed())
			MaxRange = PestilenceMovementComponent->GetHearingCrouchMaxRange();
    else if (PestilenceMovementComponent->Velocity.SizeSquared() < PestilenceMovementComponent->GetFastSpeed() * PestilenceMovementComponent->GetFastSpeed())
      MaxRange = PestilenceMovementComponent->GetHearingMediumMaxRange();
    else
      MaxRange = PestilenceMovementComponent->GetHearingFastMaxRange();

		if(MaxRange > 0.f)
			UAISense_Hearing::ReportNoiseEvent(
				GetWorld(),
				GetActorLocation(),
				1,
				this,
				MaxRange,
				UPestilenceHearingComponent::SoundTypeToString(SoundType::Step)
				);
		//MakeNoise(1,this,GetActorLocation(),MaxRange);
	}
}

void APestilenceCharacter::DetachPickUp()
{
	if (CurrentPickUp)
	{
		CurrentPickUp->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentPickUp = nullptr;
	}
}

// OnKill called after the mesh of a actor is finished dissolving and from QuickLoad()
void APestilenceCharacter::OnKill()
{
	if (!bIsDead) { bIsDead = true;	}

	InteractableComponent->SetActive(false, false);

	//Destroy(); Try later
	if (PestilenceAIController)
		PestilenceAIController->SetCurrentState(EPestilenceStates::Dead);
	ManagerReference->EnableNPC(this, Controller, false, 8);
	SetActorHiddenInGame(bIsDead);
 	SetActorEnableCollision(false);
}

void APestilenceCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!ID.IsValid())
	{
		ID = FGuid::NewGuid();
		MarkPackageDirty();
	}
}

void APestilenceCharacter::QuickSaveAction()
{
	UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
	UPestilenceSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
	APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
	
	if (!GameMode->GetChaseState())
	{
		SaveSubsystem->ResetSlotName();
		SaveSubsystem->QuickSave();
	}
}

void APestilenceCharacter::QuickLoadAction()
{
	UPestilenceGameInstance* GameInstance = GetGameInstance<UPestilenceGameInstance>();
	UPestilenceSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();

	SaveSubsystem->ResetSlotName();
	if (UGameplayStatics::DoesSaveGameExist(SaveSubsystem->GetCurrentSlotName(),0))
	{
		GameInstance->SetLoadGame(true);
		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
		FName LevelName(*CurrentLevelName);
		UGameplayStatics::OpenLevel(this, LevelName);
	}
	else
	{
		SaveSubsystem->QuickLoad(); // returns couldnt load debug
	}
}

void APestilenceCharacter::SetOverlayMaterialDetected()
{
  GetMesh()->SetOverlayMaterial(HighlightDetected);
}

void APestilenceCharacter::SetOverlayMaterialSelected()
{
  GetMesh()->SetOverlayMaterial(HighlightSelected);
}

void APestilenceCharacter::SetOverlayMaterialForbidden()
{
	GetMesh()->SetOverlayMaterial(HighlightForbidden);
}

void APestilenceCharacter::SetOverlayMaterialUsing()
{
  GetMesh()->SetOverlayMaterial(HighlightUsing);
}

void APestilenceCharacter::SetOverlayMaterialNone()
{
	GetMesh()->SetOverlayMaterial(nullptr);
}

void APestilenceCharacter::StartInfectParticles()
{
	InfectParticles->ActivateSystem();
}

void APestilenceCharacter::StopInfectParticles()
{
  InfectParticles->DeactivateImmediate();
}

void APestilenceCharacter::StartImpactParticles()
{
	ImpactParticles->ActivateSystem();
}

void APestilenceCharacter::StartTakeDownParticles()
{
  LeftTakeDownParticles->ActivateSystem();
  RightTakeDownParticles->ActivateSystem();
}

void APestilenceCharacter::StopTakeDownParticles()
{
  LeftTakeDownParticles->DeactivateImmediate();
  RightTakeDownParticles->DeactivateImmediate();
}

//void APestilenceCharacter::PossessedBy(AController* NewController)
//{
//}


// Possession Input Callbacks
void APestilenceCharacter::StartPossession() { PestilencePossessionComponent->StartPossession(); }
void APestilenceCharacter::CancelPossession() { PestilencePossessionComponent->CancelPossession(); }
void APestilenceCharacter::StartVision() { PestilencePossessionComponent->StartVision(); }
void APestilenceCharacter::CancelVision() { PestilencePossessionComponent->CancelVision(); }

void APestilenceCharacter::OnEndPossessCallback()
{
	APestilenceCharacter* Other = Cast<APestilenceCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	Other->CurrentMappingContext = CurrentMappingContext;
	UPestilenceAbilityComponent* OtherAbilityComponent = Other->GetPestilenceAbilityComponent();
	
	if (!OtherAbilityComponent)
		return;

	OtherAbilityComponent->SelectAbility(PestilenceAbilityComponent->GetCurrentAbilityId(), false);
}

void APestilenceCharacter::OnPossessed()
{
  StopInfectParticles(); 
	OnTalkRumour.Broadcast(false);
  InteractorComponent->SetActive(true);
  InteractorComponent->SearchForNearbyInteractables();
  GetMesh1P()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

bool APestilenceCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const
{
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if( PlayerController == nullptr) return false;
	
	FVector TargetLocation = ObserverLocation;
	TargetLocation+= CanBeSeenFromOffset;

	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(LineOfSight), true, this);
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(PlayerController->GetPawn());

	FVector Start;
	FRotator Rotation;
	GetActorEyesViewPoint(Start,Rotation);
	bool bHit = GetWorld()->LineTraceTestByChannel(Start, TargetLocation, ECC_Visibility, CollisionParams);

	return !bHit;
}
