// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).


#include "Cpp_Ch_K53sc.h"

// Sets default values
ACpp_Ch_K53sc::ACpp_Ch_K53sc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACpp_Ch_K53sc::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACpp_Ch_K53sc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACpp_Ch_K53sc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
