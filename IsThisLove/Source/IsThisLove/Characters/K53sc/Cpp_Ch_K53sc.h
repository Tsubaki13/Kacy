// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cpp_Ch_K53sc.generated.h"

UCLASS()
class ISTHISLOVE_API ACpp_Ch_K53sc : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACpp_Ch_K53sc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicComponents")
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicComponents")
		class UCameraComponent* FirstPersonCam;

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void LookUp(float Amount);
	void Turn(float Amount);
	void PerformJump();
	void Interact();

	USkeletalMeshComponent* SkelMesh;
};
