// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cpp_InspectionComp.generated.h"

class ACpp_Ch_K53sc;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class KACY_API UCpp_InspectionComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCpp_InspectionComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Item Inspection")
		int32 InspectionTraceRange; // how far should the trace looking for objects extend
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Item Inspection")
		int32 InspectedItemDistanceFromCam; // how far the object is from camera while inspecting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Item Inspection")
		float InspectionScreenSize; // this is the factor used in calculating the scale of the inspected item as it appears on screen while being inspected
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Kacy - Item Inspection")
		void MoveItemToInspectionLoc(AActor* InspectedItem, FTransform ItemOriginalTransform, FVector InspectedItemLoc, FRotator InspectedItemRot, FVector InspectedItemScale); // called to make the object lerp from its original location to the inspection location via a timeline in blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Kacy - Item Inspection")
		void MoveItemToOriginalLoc(); // just like the function before it but reversed

	bool bTraceHitActor; // true if trace hits an actor
	bool bItemIsInspectable; // checks if the item hit by the trace has the "inspectable" tag
	AActor* InspectedItem; // a reference to the inspected item
	FTransform ItemOriginalTransform; // the original transform of the item before inspection

	void InspectItem(); // manages the inspection process
	FHitResult InteractionTrace(); // trace from the player's eye in front of it in a range whose value can be set from the editor
	void SetItemInspectionTransform(); // bring the item close to the camera if the interaction trace hits something
	FVector SetItemScale(); // calculates the scale and feeds it into the previous function
	void RestoreItemTransform(FTransform ItemOriginalTransform); // restore the item to its original transform after inspecting it
	bool bIsCurrentlyInspectingItem;
	void SetInspectedItem(AActor *ItemToInspect);

	UWorld* MyWorld = GetWorld(); // a reference to the world
};
