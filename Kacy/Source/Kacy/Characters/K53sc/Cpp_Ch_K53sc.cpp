// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

#include "Cpp_Ch_K53sc.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "CppAnim_K53sc.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"

// Sets default values
ACpp_Ch_K53sc::ACpp_Ch_K53sc()
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
		UE_LOG(LogTemp, Warning, TEXT("speed: %f / %s"), AnimInstance->Speed, (AnimInstance->bIsFalling ? TEXT("is falling") : TEXT("is NOT falling")))
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
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ACpp_Ch_K53sc::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
}

void ACpp_Ch_K53sc::LookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void ACpp_Ch_K53sc::Turn(float Amount)
{
	AddControllerYawInput(Amount);
}

void ACpp_Ch_K53sc::PerformJump()
{
	Jump();
}

void ACpp_Ch_K53sc::Interact()
{

}
