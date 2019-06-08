// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cpp_InspectionComp.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KACY_API UCpp_InspectionComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCpp_InspectionComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Kacy - Item Inspection")
		int32 InspectionTraceRange;
	UPROPERTY(EditAnywhere, Category = "Kacy - Item Inspection")
		int32 InspectedItemDistanceFromCam;

	bool bTraceHitActor; // true if trace hits an actor
	bool bItemIsInspectable; // checks if the item hit by the trace has the "inspectable" tag
	AActor* InspectedItem; // a reference to the inspected item
	FTransform ItemOriginalTransform; // the original transform of the item before inspection

	void InspectItem(); // manages the inspection process
	FHitResult InteractionTrace(); // trace from the player's eye in front of it in a range whose value can be set from the editor
	void SetItemInspectionLoc(FHitResult HitResult); // bring the item close to the camera if the interaction trace hits something
	void RestoreItemTransform(FTransform ItemOriginalTransform); // restore the item to its original transform after inspecting it


	UWorld* MyWorld = GetWorld(); // a reference to the world
};
