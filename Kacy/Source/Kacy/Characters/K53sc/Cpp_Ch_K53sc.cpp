// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#include "Cpp_Ch_K53sc.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "CppAnim_K53sc.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
#include "Cpp_InspectionComp.h"
#include "Cpp_PickupComp.h"
#include "Classes/GameFramework/Actor.h"
//#include "Public/DrawDebugHelpers.h"

// Sets default values
ACpp_Ch_K53sc::ACpp_Ch_K53sc()
{
	PrimaryActorTick.bCanEverTick = true;

	// creating default components and setting some of their properties as needed:
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	SkelMesh = GetMesh();
	CameraBoom->SetupAttachment(SkelMesh);
	CameraBoom->bUsePawnControlRotation = true;

	FirstPersonCam = CreateDefaultSubobject<UCameraComponent>("FirstPersonCam");
	FirstPersonCam->SetupAttachment(CameraBoom);
}

void ACpp_Ch_K53sc::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UCppAnim_K53sc>(SkelMesh->GetAnimInstance());
}

void ACpp_Ch_K53sc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(AnimInstance && SkelMesh)
	{
		CurrentVelocity = AnimInstance->TryGetPawnOwner()->GetVelocity();
		AnimInstance->Velocity = CurrentVelocity;
		bIsCurrentlyFalling = ACharacter::GetCharacterMovement()->IsFalling();
		AnimInstance->bIsFalling = bIsCurrentlyFalling;
		AnimInstance->bIsInspecting = InspectionComponent->bIsCurrentlyInspectingItem;
		//UE_LOG(LogTemp, Warning, TEXT("speed: %f / %s"), AnimInstance->Speed, (AnimInstance->bIsFalling ? TEXT("is falling") : TEXT("is NOT falling")))
	}
	else
	{
		if(!SkelMesh)
			UE_LOG(LogTemp, Warning, TEXT("SkelMesh ref missing"))
		if(!AnimInstance)
			UE_LOG(LogTemp, Warning, TEXT("AnimInstance ref missing"))
	}
}

void ACpp_Ch_K53sc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACpp_Ch_K53sc::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACpp_Ch_K53sc::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACpp_Ch_K53sc::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ACpp_Ch_K53sc::Turn);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACpp_Ch_K53sc::PerformJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACpp_Ch_K53sc::Interact);
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &ACpp_Ch_K53sc::Grab);
	PlayerInputComponent->BindAction("Grab", IE_Released, this, &ACpp_Ch_K53sc::Ungrab);
	PlayerInputComponent->BindAction("DropItem", IE_Released, this, &ACpp_Ch_K53sc::DropItem);
}

void ACpp_Ch_K53sc::SetInspectionCompRef(UCpp_InspectionComp* InspectionComponentToSet)
{
	InspectionComponent = InspectionComponentToSet;
}

void ACpp_Ch_K53sc::SetPickupCompRef(UCpp_PickupComp* PickupComponentToSet)
{
	PickupComponent = PickupComponentToSet;
}

void ACpp_Ch_K53sc::MoveForward(float Amount)
{
	if (InspectionComponent && !InspectionComponent->InspectedItem)
	{
		AddMovementInput(GetActorForwardVector(), Amount);
	}
	else if (InspectionComponent && InspectionComponent->bItemIsInspectable && (Amount > 0) || (Amount < 0))
	{
		FRotator InspectionRotation = FRotator(-Amount * .5f, 0.f, 0.f);
		InspectionComponent->InspectedItem->AddActorWorldRotation(InspectionRotation, false);
	}
}

void ACpp_Ch_K53sc::MoveRight(float Amount)
{
	if(InspectionComponent && !InspectionComponent->InspectedItem)
	{
		AddMovementInput(GetActorRightVector(), Amount);
	}
	else if(InspectionComponent && InspectionComponent->bItemIsInspectable && (Amount > 0) || (Amount < 0))
	{
		FRotator InspectionRotation = FRotator(0.f, -Amount * .5f, 0.f);
		InspectionComponent->InspectedItem->AddActorWorldRotation(InspectionRotation, false);
	}
}

void ACpp_Ch_K53sc::LookUp(float Amount)
{
	if(InspectionComponent && !InspectionComponent->InspectedItem)
	{
		AddControllerPitchInput(Amount);
	}
}

void ACpp_Ch_K53sc::Turn(float Amount)
{
	if(InspectionComponent && !InspectionComponent->InspectedItem)
	{
		AddControllerYawInput(Amount);
	}
}

void ACpp_Ch_K53sc::PerformJump()
{
	if(InspectionComponent && !InspectionComponent->InspectedItem)
	{
		Jump();
	}
}

void ACpp_Ch_K53sc::Interact()
{
	if (!bIsCurrentlyFalling)
	{
		if (InspectionComponent)
		{
			if (!InspectionComponent->InspectedItem)
			{
				FHitResult TraceResult = LookForActor();
				if (TraceResult.GetActor() && TraceResult.GetActor()->ActorHasTag("Inspectable"))
				{
					InspectionComponent->SetInspectedItem(TraceResult.GetActor());
					InspectionComponent->InspectItem();
				}
			}
			else
			{
				InspectionComponent->RestoreItemTransform(InspectionComponent->ItemOriginalTransform);
				InspectionComponent->InspectedItem = nullptr;
				InspectionComponent->bItemIsInspectable = false;
			}
		}
	}
}

void ACpp_Ch_K53sc::Grab()
{
	if(!bIsCurrentlyFalling)
	{
		if(InspectionComponent && PickupComponent)
		{
			if(!InspectionComponent->bIsCurrentlyInspectingItem)
			{
				OnGrabItem.Broadcast();

				FHitResult TraceResult = LookForActor();
				if(TraceResult.GetActor())
					PickupComponent->PickupItem(TraceResult.GetActor());
			}
			else
			{
				if(PickupComponent && InspectionComponent->bIsCurrentlyInspectingItem)
				{
					PickupComponent->PickupItem(InspectionComponent->InspectedItem);

					if (PickupComponent->bItemIsPickupable)
					{
						InspectionComponent->bIsCurrentlyInspectingItem = false;
						InspectionComponent->InspectedItem->SetActorScale3D(InspectionComponent->ItemOriginalTransform.GetScale3D());
						InspectionComponent->InspectedItem = nullptr;
					}
				}
			}
		}
	}
}

FHitResult ACpp_Ch_K53sc::LookForActor()
{
	FHitResult HitResult;
	FVector PlayerViewLoc;
	FRotator PlayerViewRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLoc, PlayerViewRot); // this function sets the out parameters of the location of the player's "eye" and its rotation
	FVector InteractLineEnd = PlayerViewLoc + (PlayerViewRot.Vector() * InspectionComponent->InspectionTraceRange);
	FCollisionQueryParams ColParams;
	ColParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, PlayerViewLoc, InteractLineEnd, ECC_WorldDynamic, ColParams);
	//DrawDebugLine(GetWorld(), PlayerViewLoc, InteractLineEnd, FColor(255, 0, 0), false, 2.f, 0.f, 5.f); // drawing debug line only. NOT the actual trace

	return HitResult;
}

void ACpp_Ch_K53sc::Ungrab()
{
	OnUngrabItem.Broadcast();
}

void ACpp_Ch_K53sc::DropItem()
{
	if (PickupComponent)
	{
		if (InspectionComponent && !InspectionComponent->bIsCurrentlyInspectingItem && !bIsCurrentlyFalling)
		{
			PickupComponent->DropItem();
		}
	}
}
