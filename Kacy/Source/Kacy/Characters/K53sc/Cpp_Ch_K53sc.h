// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cpp_Ch_K53sc.generated.h"

UCLASS()
class KACY_API ACpp_Ch_K53sc : public ACharacter
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - BasicComponents")
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - BasicComponents")
		class UCameraComponent* FirstPersonCam;

	UPROPERTY(EditAnywhere, Category = "Kacy - Item Inspection")
		int32 InspectionTraceRange;
	UPROPERTY(EditAnywhere, Category = "Kacy - Item Inspection")
		int32 InspectedItemDistanceFromCam;

private:
	// the following methods are used to setup input by getting called from SetupPlayerInputComponent method
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void LookUp(float Amount);
	void Turn(float Amount);
	void PerformJump();
	void Interact();

	void InspectItem(); // manages the inspection process

	FHitResult InteractionTrace(); // trace from the player's eye in front of it in a range whose value can be set from the editor
	bool bTraceHitActor; // true if trace hits an actor
	bool bItemIsInspectable; // checks if the item hit by the trace has the "inspectable" tag
	AActor* InspectedItem; // a reference to the inspected item
	void SetItemInspectionLoc(FHitResult HitResult); // bring the item close to the camera if the interaction trace hits something
	FTransform ItemOriginalTransform; // the original transform of the item before inspection
	void RestoreItemTransform(FTransform ItemOriginalTransform); // restore the item to its original transform after inspecting it

	UWorld* MyWorld = GetWorld(); // a reference to the world
	USkeletalMeshComponent* SkelMesh; // a reference to the skeletal mesh
	class UCppAnim_K53sc* AnimInstance; // a reference to the animation instance class, from which the animation blueprint inherits
};
