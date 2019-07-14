// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cpp_PickupComp.generated.h"

class UCpp_InspectionComp;
class ACpp_Ch_K53sc;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class KACY_API UCpp_PickupComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCpp_PickupComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PickupItem(AActor *ItemToPickup); // moves the item from the inspection screen to hand if there's space
	void DropItem(); // lets go of the item in hand and moves the item on the back to the hand. if no item on the back, only lets go of it

	int32 NumberOfHeldItems;
	bool bHasItemInHand, bHasItemOnBack, bItemIsPickupable;

	UFUNCTION(BlueprintImplementableEvent, Category = "Kacy - Pickupables")
		void ItemPickedUp(AActor* PickedUpItem, bool bPicked);
	UFUNCTION(BlueprintImplementableEvent, Category = "Kacy - Pickupables")
		void DecreaseNumberOfBatteriesHeld();
	UFUNCTION(BlueprintCallable, Category = "Kacy - Pickupables")
		void RemoveBatteries(AActor* Battery);


private:
	UCpp_InspectionComp* InspectionComponent; // a reference to the inspection component

	void PlaceItemOnFloor(AActor* DroppedItem);
	void AttachItemToSocket(AActor* ItemToPickup, FName SocketName);
	
	AActor* ItemInHand;
	AActor* ItemOnBack;
	AActor* Battery;

	int32 NumberOfItemsHeld;
	UWorld* MyWorld;
	ACpp_Ch_K53sc* K53sc;
};
