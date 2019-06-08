// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cpp_Ch_K53sc.generated.h"

class UCpp_InspectionComp;

UCLASS()
class KACY_API ACpp_Ch_K53sc : public ACharacter
{
	GENERATED_BODY()

public:
	ACpp_Ch_K53sc();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - BasicComponents")
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - BasicComponents")
		class UCameraComponent* FirstPersonCam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Item Inspection")
		UCpp_InspectionComp* InspectionComponent;

private:
	// the following methods are used to setup input by getting called from SetupPlayerInputComponent method
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void LookUp(float Amount);
	void Turn(float Amount);
	void PerformJump();
	void Interact();

	USkeletalMeshComponent* SkelMesh; // a reference to the skeletal mesh
	class UCppAnim_K53sc* AnimInstance; // a reference to the animation instance class, from which the animation blueprint inherits
};
