// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

#include "Cpp_Ch_K53sc.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "CppAnim_K53sc.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
//#include "Public/DrawDebugHelpers.h"
#include "Classes/GameFramework/Actor.h"

// Sets default values
ACpp_Ch_K53sc::ACpp_Ch_K53sc() :
	InspectionTraceRange(100),
	InspectedItemDistanceFromCam(100),
	bTraceHitActor(false),
	bItemIsInspectable(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	SkelMesh = GetMesh();
	CameraBoom->SetupAttachment(SkelMesh);
	CameraBoom->bUsePawnControlRotation = true;

	FirstPersonCam = CreateDefaultSubobject<UCameraComponent>("FirstPersonCam");
	FirstPersonCam->SetupAttachment(CameraBoom);
}

// Called when the game starts or when spawned
void ACpp_Ch_K53sc::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UCppAnim_K53sc>(SkelMesh->GetAnimInstance());
}

// Called every frame
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

// Called to bind functionality to input
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

void ACpp_Ch_K53sc::MoveForward(float Amount)
{
	if (!bItemIsInspectable)
	{
		AddMovementInput(GetActorForwardVector(), Amount);
	}
}

void ACpp_Ch_K53sc::MoveRight(float Amount)
{
	if(!bItemIsInspectable)
	{
		AddMovementInput(GetActorRightVector(), Amount);
	}
}

void ACpp_Ch_K53sc::LookUp(float Amount)
{
	if(!bItemIsInspectable)
	{
		AddControllerPitchInput(Amount);
	}
}

void ACpp_Ch_K53sc::Turn(float Amount)
{
	if(!bItemIsInspectable)
	{
		AddControllerYawInput(Amount);
	}
}

void ACpp_Ch_K53sc::PerformJump()
{
	if(!bItemIsInspectable)
	{
		Jump();
	}
}

void ACpp_Ch_K53sc::Interact()
{
	if(!bTraceHitActor)
	{
		InspectItem();
		if(!bItemIsInspectable)
			bTraceHitActor = false;
	}
	else
	{
		if (bItemIsInspectable)
		{
			RestoreItemTransform(ItemOriginalTransform);
			bTraceHitActor = false;
			bItemIsInspectable = false;
		}
	}
}

void ACpp_Ch_K53sc::InspectItem()
{
	FHitResult HitResult = InteractionTrace();
	if (InspectedItem && bItemIsInspectable)
	{
		ItemOriginalTransform = InspectedItem->GetTransform();
		SetItemInspectionLoc(HitResult);
	}
}

FHitResult ACpp_Ch_K53sc::InteractionTrace()
{
	// creating the out parameters needed for the line trace.
	FHitResult HitResult;
	FVector PlayerViewLoc;
	FRotator PlayerViewRot;
	MyWorld->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLoc, PlayerViewRot); // this function sets the out parameters of the location of the player's "eye" and its rotation
	FVector InteractLineEnd = PlayerViewLoc + (PlayerViewRot.Vector() * InspectionTraceRange);
	FCollisionQueryParams ColParams;
	ColParams.AddIgnoredActor(this);
	
	bTraceHitActor = (MyWorld->LineTraceSingleByChannel(HitResult, PlayerViewLoc, InteractLineEnd, ECC_WorldDynamic, ColParams));

	if(bTraceHitActor)
	{
		InspectedItem = HitResult.GetActor();
		bItemIsInspectable = InspectedItem->ActorHasTag("Inspectable");
	}
	return HitResult;
	/*if (bInspectingItem) // line trace returns true if hit something
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s in %s"), *HitResult.GetActor()->GetName(), *HitResult.Location.ToString()); // log just to report the hit actor name and the location of the hit
		DrawDebugLine(MyWorld, InteractLineStart, InteractLineEnd, FColor(255, 0, 0), false, 2.f, 0.f, 5.f); // drawing debug line only. NOT the actual trace
	}*/
}

void ACpp_Ch_K53sc::SetItemInspectionLoc(FHitResult HitResult)
{
	FVector PlayerViewLoc, InspectedItemLoc;
	FRotator PlayerViewRot;
	MyWorld->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLoc, PlayerViewRot);
	InspectedItemLoc = PlayerViewLoc + (PlayerViewRot.Vector() * InspectedItemDistanceFromCam);

	InspectedItem->SetActorLocation(InspectedItemLoc);
}

void ACpp_Ch_K53sc::RestoreItemTransform(FTransform ItemOriginalTransform)
{
	if (InspectedItem != nullptr)
	{
		InspectedItem->SetActorTransform(ItemOriginalTransform);
	}
}
