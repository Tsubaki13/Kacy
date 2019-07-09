// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#include "Cpp_Ch_K53sc.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "CppAnim_K53sc.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
#include "Cpp_InspectionComp.h"
#include "Cpp_PickupComp.h"
#include "Cpp_PushComponent.h"
#include "Actors/Cpp_InteractableItem.h"
#include "Classes/GameFramework/Actor.h"
//#include "Public/DrawDebugHelpers.h"

ACpp_Ch_K53sc::ACpp_Ch_K53sc()
{
	PrimaryActorTick.bCanEverTick = true;

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
		AnimInstance->bIsPushing = PushComponent->bIsPushing;
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
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ACpp_Ch_K53sc::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ACpp_Ch_K53sc::ZoomOut);
}

void ACpp_Ch_K53sc::SetInspectionCompRef(UCpp_InspectionComp* InspectionComponentToSet)
{
	InspectionComponent = InspectionComponentToSet;
}
void ACpp_Ch_K53sc::SetPickupCompRef(UCpp_PickupComp* PickupComponentToSet)
{
	PickupComponent = PickupComponentToSet;
}
void ACpp_Ch_K53sc::SetPushCompRef(UCpp_PushComponent* PushComponentToSet)
{
	PushComponent = PushComponentToSet;
}
void ACpp_Ch_K53sc::MoveForward(float Amount)
{
	if(InspectionComponent && !InspectionComponent->InspectedItem)
	{
		AddMovementInput(GetActorForwardVector(), Amount);
	}
	else if(InspectionComponent && InspectionComponent->bItemIsInspectable && (Amount > 0) || (Amount < 0))
	{
		InspectionComponent->RotateInspectedItem(Amount, true);
	}
	
	if(PushComponent && PushComponent->bIsPushing)
	{
		PushComponent->MovePushedItem(Amount);
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
		InspectionComponent->RotateInspectedItem(Amount, false);
	}
}
void ACpp_Ch_K53sc::LookUp(float Amount)
{
	if(InspectionComponent && PushComponent
		&& !InspectionComponent->InspectedItem)
	{
		AddControllerPitchInput(Amount);
	}
}
void ACpp_Ch_K53sc::Turn(float Amount)
{
	if(InspectionComponent && PushComponent
		&& !InspectionComponent->InspectedItem)
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
				InspectionComponent->InspectedItemDistanceFromCam = 100;
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
void ACpp_Ch_K53sc::ZoomIn()
{
	if(InspectionComponent && InspectionComponent->bIsCurrentlyInspectingItem && InspectionComponent->InspectedItemDistanceFromCam > 35)
	{
		InspectionComponent->InspectedItemDistanceFromCam -= 3;

		FVector PlayerViewLoc, InspectedItemLoc;
		FRotator PlayerViewRot;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLoc, PlayerViewRot);
		InspectedItemLoc = PlayerViewLoc + (PlayerViewRot.Vector() * InspectionComponent->InspectedItemDistanceFromCam);

		InspectionComponent->InspectedItem->SetActorLocation(InspectedItemLoc);
	}
}
void ACpp_Ch_K53sc::ZoomOut()
{
	if(InspectionComponent && InspectionComponent->bIsCurrentlyInspectingItem && InspectionComponent->InspectedItemDistanceFromCam < 200)
	{
		InspectionComponent->InspectedItemDistanceFromCam += 3;

		FVector PlayerViewLoc, InspectedItemLoc;
		FRotator PlayerViewRot;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLoc, PlayerViewRot);
		InspectedItemLoc = PlayerViewLoc + (PlayerViewRot.Vector() * InspectionComponent->InspectedItemDistanceFromCam);

		InspectionComponent->InspectedItem->SetActorLocation(InspectedItemLoc);
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
