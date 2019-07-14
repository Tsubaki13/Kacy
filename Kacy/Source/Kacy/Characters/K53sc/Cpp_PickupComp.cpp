// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#include "Cpp_PickupComp.h"
#include "Cpp_Ch_K53sc.h"
#include "Cpp_InspectionComp.h"
#include "Classes/Components/SkeletalMeshComponent.h"
// #include "Public/DrawDebugHelpers.h"

UCpp_PickupComp::UCpp_PickupComp() :
	NumberOfHeldItems(0),
	bHasItemInHand(false),
	bHasItemOnBack(false),
	bItemIsPickupable(false),
	NumberOfItemsHeld(0)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCpp_PickupComp::BeginPlay()
{
	Super::BeginPlay();

	MyWorld = GetWorld();
	K53sc = Cast<ACpp_Ch_K53sc>(MyWorld->GetFirstPlayerController()->GetPawn());
	InspectionComponent = Cast<UCpp_InspectionComp>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UCpp_InspectionComp::StaticClass()));
}

void UCpp_PickupComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCpp_PickupComp::PickupItem(AActor* ItemToPickup)
{
	bItemIsPickupable = ItemToPickup->ActorHasTag("Pickupable");
	if (bItemIsPickupable)
	{
		if (!bHasItemInHand && !ItemInHand && NumberOfItemsHeld < 1)
		{
			AttachItemToSocket(ItemToPickup, "RHandSocket");

			ItemInHand = ItemToPickup;
			bHasItemInHand = true;
			NumberOfItemsHeld++;
		}
		else if (!bHasItemOnBack && !ItemOnBack && NumberOfItemsHeld < 2)
		{
			AttachItemToSocket(ItemToPickup, "BackSocket");

			ItemOnBack = ItemToPickup;
			bHasItemOnBack = true;
			NumberOfItemsHeld++;
		}
		else if (NumberOfItemsHeld == 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("CAN'T PICKUP ANYTHING ANYMORE!"))
		}
	}
}

void UCpp_PickupComp::AttachItemToSocket(AActor* ItemToPickup, FName SocketName)
{
	USkeletalMeshComponent* SkMesh = Cast<USkeletalMeshComponent>(K53sc->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	FAttachmentTransformRules AttachRules = AttachRules.SnapToTargetNotIncludingScale;
	ItemToPickup->AttachToComponent(SkMesh, AttachRules, SocketName);
	ItemToPickup->SetActorEnableCollision(false);

	ItemPickedUp(ItemToPickup, true);
}

void UCpp_PickupComp::DropItem()
{
	USkeletalMeshComponent* SkMesh = Cast<USkeletalMeshComponent>(K53sc->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (ItemInHand && NumberOfItemsHeld >= 1)
	{
		FDetachmentTransformRules DetachRules = DetachRules.KeepWorldTransform;
		ItemInHand->DetachFromActor(DetachRules);
		ItemInHand->SetActorEnableCollision(true);
		PlaceItemOnFloor(ItemInHand);
		bHasItemInHand = false;

		if (ItemOnBack && NumberOfItemsHeld == 2)
		{
			FAttachmentTransformRules AttachRules = AttachRules.SnapToTargetNotIncludingScale;
			ItemOnBack->AttachToComponent(SkMesh, AttachRules, "RHandSocket");
			bHasItemInHand = true;
			ItemInHand = ItemOnBack;
			ItemPickedUp(ItemOnBack, false);
			ItemOnBack = nullptr;
			bHasItemOnBack = false;
			NumberOfItemsHeld--;

			return;
		}

		ItemPickedUp(ItemInHand, false);
		ItemInHand = nullptr;
		NumberOfItemsHeld--;
	}
}

void UCpp_PickupComp::PlaceItemOnFloor(AActor* DroppedItem)
{
	FHitResult HitResult;
	FVector TraceStart = DroppedItem->GetActorLocation();
	FVector TraceEnd = DroppedItem->GetActorLocation() + FVector(0.f, 0.f, -800.f);
	FCollisionQueryParams ColParams;
	ColParams.AddIgnoredActor(K53sc);
	ColParams.AddIgnoredActor(DroppedItem);

	// DrawDebugLine(MyWorld, TraceStart, TraceEnd, FColor(255, 0, 0), false, 2.f, 0.f, 5.f);
	if (MyWorld->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, ColParams))
	{
		float MeshRadius = Cast<UStaticMeshComponent>(DroppedItem->GetComponentByClass(UStaticMeshComponent::StaticClass()))->Bounds.SphereRadius;
		DroppedItem->SetActorLocation(HitResult.Location + FVector(MeshRadius * .65f), true);
	}
}
