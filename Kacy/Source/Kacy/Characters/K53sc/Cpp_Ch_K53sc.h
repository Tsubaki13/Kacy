// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cpp_Ch_K53sc.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrabDelegate); // event dispatcher created to be called from any blueprint

class UCpp_InspectionComp; // declaration of the inspection component class
class UCpp_PickupComp; // declaration of the pickup component class
class UCpp_PushComponent; // declaration of the push component class
class ACpp_InteractableItem;

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

	// the following UPROPERTYs are used to create the default needed components in the character class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - BasicComponents")
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - BasicComponents")
		class UCameraComponent* FirstPersonCam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Item Inspection")
		UCpp_InspectionComp* InspectionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Item Pickup")
		UCpp_PickupComp* PickupComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Item Pushing")
		UCpp_PushComponent* PushComponent;

	// the following UPROPERTYs are of the type FGrabDelegate which is the event dispatcher created at the top of this file. they will be broadcasted to blueprint
	UPROPERTY(BlueprintAssignable, Category = "Kacy - Grab")
		FGrabDelegate OnGrabItem;
	UPROPERTY(BlueprintAssignable, Category = "Kacy - Grab")
		FGrabDelegate OnUngrabItem;

	UFUNCTION(BlueprintCallable, Category = "Kacy - Item Inspection") // this method is called to set a reference to the InspectionComponent that is added to the bp
		void SetInspectionCompRef(UCpp_InspectionComp* InspectionComponentToSet);
	UFUNCTION(BlueprintCallable, Category = "Kacy - Item Pickup") // this method is called to set a reference to the PickupComponent that is added to the bp
		void SetPickupCompRef(UCpp_PickupComp* PickupComponentToSet);
	UFUNCTION(BlueprintCallable, Category = "Kacy - Item Pushing") // this method is called to set a reference to the PushComponent that is added to the bp
		void SetPushCompRef(UCpp_PushComponent* PushComponentToSet);

	// the following variables will be used to set the values in the animation instance from which the animation blueprint inherits
	FVector CurrentVelocity;
	bool bIsCurrentlyFalling;

private:
	// the following methods are used to setup input by getting called from SetupPlayerInputComponent method
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void LookUp(float Amount);
	void Turn(float Amount);
	void PerformJump();
	void Interact();
	void Grab();
	void Ungrab();
	void DropItem();
	void ZoomIn();
	void ZoomOut();

	FHitResult LookForActor(); // helper function

	USkeletalMeshComponent* SkelMesh; // a reference to the skeletal mesh
	class UCppAnim_K53sc* AnimInstance; // a reference to the animation instance class, from which the animation blueprint inherits
};
 