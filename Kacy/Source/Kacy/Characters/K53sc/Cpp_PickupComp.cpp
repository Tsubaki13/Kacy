// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#include "Cpp_PickupComp.h"
#include "Cpp_Ch_K53sc.h"
#include "Cpp_InspectionComp.h"
#include "Classes/Components/SkeletalMeshComponent.h"

UCpp_PickupComp::UCpp_PickupComp() :
	NumberOfHeldItems(0),
	bHasItemInHand(false),
	bHasItemOnBack(false)
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

void UCpp_PickupComp::PickupItem()
{
	if (!bHasItemInHand)
	{
		InspectionComponent->bTraceHitActor = false;
		InspectionComponent->bItemIsInspectable = false;
		USkeletalMeshComponent* SkMesh = Cast<USkeletalMeshComponent>(K53sc->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		
		InspectionComponent->InspectedItem->SetActorEnableCollision(false);
		InspectionComponent->InspectedItem->SetActorScale3D(InspectionComponent->ItemOriginalTransform.GetScale3D());

		FAttachmentTransformRules AttachRules = AttachRules.SnapToTargetNotIncludingScale;
		InspectionComponent->InspectedItem->AttachToComponent(SkMesh, AttachRules, "RHandSocket");

		bHasItemInHand = true;
		InspectionComponent->bIsCurrentlyInspectingItem = false;
		ItemInHand = InspectionComponent->InspectedItem;
	}
	else if (!bHasItemOnBack)
	{
		InspectionComponent->bTraceHitActor = false;
		InspectionComponent->bItemIsInspectable = false;
		USkeletalMeshComponent* SkMesh = Cast<USkeletalMeshComponent>(K53sc->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

		InspectionComponent->InspectedItem->SetActorEnableCollision(false);
		InspectionComponent->InspectedItem->SetActorScale3D(InspectionComponent->ItemOriginalTransform.GetScale3D());

		FAttachmentTransformRules AttachRules = AttachRules.SnapToTargetNotIncludingScale;
		InspectionComponent->InspectedItem->AttachToComponent(SkMesh, AttachRules, "BackSocket");

		bHasItemOnBack = true;
		InspectionComponent->bIsCurrentlyInspectingItem = false;
		ItemOnBack = InspectionComponent->InspectedItem;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CAN'T PICKUP ANYTHING ANYMORE! ALL IS FULL!!"));
	}
}

void UCpp_PickupComp::DropItem()
{
		USkeletalMeshComponent* SkMesh = Cast<USkeletalMeshComponent>(K53sc->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (ItemInHand)
	{
		FDetachmentTransformRules DetachRules = DetachRules.KeepWorldTransform;
		ItemInHand->DetachFromActor(DetachRules);
		ItemInHand->SetActorEnableCollision(true);
		ItemInHand = nullptr;
		bHasItemInHand = false;

		if (ItemOnBack)
		{
			FAttachmentTransformRules AttachRules = AttachRules.SnapToTargetNotIncludingScale;
			ItemOnBack->AttachToComponent(SkMesh, AttachRules, "RHandSocket");
			bHasItemInHand = true;
			ItemInHand = ItemOnBack;
			ItemOnBack = nullptr;
			bHasItemOnBack = false;
		}
	}
}
