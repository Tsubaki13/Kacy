// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

#include "Cpp_Ch_K53sc.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "CppAnim_K53sc.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
#include "Cpp_InspectionComp.h"
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
		AnimInstance->Speed = AnimInstance->TryGetPawnOwner()->GetVelocity().Size();

		AnimInstance->bIsFalling = ACharacter::GetCharacterMovement()->IsFalling();
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
}

void ACpp_Ch_K53sc::SetInspectionCompRef(UCpp_InspectionComp* InspectionComponentToSet)
{
	InspectionComponent = InspectionComponentToSet;
}

void ACpp_Ch_K53sc::MoveForward(float Amount)
{
	if (InspectionComponent && !InspectionComponent->bItemIsInspectable)
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
	if(InspectionComponent && !InspectionComponent->bItemIsInspectable)
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
	if(InspectionComponent && !InspectionComponent->bItemIsInspectable)
	{
		AddControllerPitchInput(Amount);
	}
}

void ACpp_Ch_K53sc::Turn(float Amount)
{
	if(InspectionComponent && !InspectionComponent->bItemIsInspectable)
	{
		AddControllerYawInput(Amount);
	}
}

void ACpp_Ch_K53sc::PerformJump()
{
	if(InspectionComponent && !InspectionComponent->bItemIsInspectable)
	{
		Jump();
	}
}

void ACpp_Ch_K53sc::Interact()
{
	if(InspectionComponent && !InspectionComponent->bTraceHitActor)
	{
		InspectionComponent->InspectItem();
		if(!InspectionComponent->bItemIsInspectable)
			InspectionComponent->bTraceHitActor = false;
	}
	else
	{
		if (InspectionComponent && InspectionComponent->bItemIsInspectable)
		{
			InspectionComponent->RestoreItemTransform(InspectionComponent->ItemOriginalTransform);
			InspectionComponent->bTraceHitActor = false;
			InspectionComponent->bItemIsInspectable = false;
		}
	}
}
