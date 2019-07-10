// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#include "Cpp_InspectionComp.h"
#include "Cpp_Ch_K53sc.h"
#include "Classes/Components/CapsuleComponent.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Public/Math/BoxSphereBounds.h"
#include "Public/Components/WidgetComponent.h"
// #include "Public/DrawDebugHelpers.h"

UCpp_InspectionComp::UCpp_InspectionComp() :
	InspectionTraceRange(100),
	InspectedItemDistanceFromCam(100),
	InspectionScreenSize(10.f),
	bTraceHitActor(false),
	bItemIsInspectable(false),
	bIsCurrentlyInspectingItem(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCpp_InspectionComp::BeginPlay()
{
	Super::BeginPlay();
}

void UCpp_InspectionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCpp_InspectionComp::InspectItem()
{
	if(InspectedItem && bItemIsInspectable)
	{
		bIsCurrentlyInspectingItem = true;		
		ItemOriginalTransform = InspectedItem->GetTransform();
		SetItemInspectionTransform();

		InteractableItemIcon = Cast<UWidgetComponent>(InspectedItem->GetComponentByClass(UWidgetComponent::StaticClass()));
		if (InteractableItemIcon)
		{
			InteractableItemIcon->SetVisibility(false);
		}
	}
}

FHitResult UCpp_InspectionComp::InteractionTrace()
{
	// creating the out parameters needed for the line trace.
	FHitResult HitResult;
	FVector PlayerViewLoc;
	FRotator PlayerViewRot;
	MyWorld->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLoc, PlayerViewRot); // this function sets the out parameters of the location of the player's "eye" and its rotation
	FVector InteractLineEnd = PlayerViewLoc + (PlayerViewRot.Vector() * InspectionTraceRange);
	FCollisionQueryParams ColParams;
	ACpp_Ch_K53sc* K53sc = Cast<ACpp_Ch_K53sc>(MyWorld->GetFirstPlayerController()->GetPawn());
	if (K53sc)
	{
		ColParams.AddIgnoredActor(K53sc);
	}

	bTraceHitActor = (MyWorld->LineTraceSingleByChannel(HitResult, PlayerViewLoc, InteractLineEnd, ECC_WorldDynamic, ColParams));

	if(bTraceHitActor)
	{
		InspectedItem = HitResult.GetActor();
		bItemIsInspectable = InspectedItem->ActorHasTag("Inspectable");
	}
	
	return HitResult;

	/*if (bInspectingItem) // line trace returns true if hit something
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s in %s"), *HitResult.GetActor()->GetName(), *HitResult.Location.ToString()); // log just to report the hit actor name and the location of the hit
		DrawDebugLine(MyWorld, InteractLineStart, InteractLineEnd, FColor(255, 0, 0), false, 2.f, 0.f, 5.f); // drawing debug line only. NOT the actual trace
	}*/
}

void UCpp_InspectionComp::SetItemInspectionTransform()
{
	FVector PlayerViewLoc, InspectedItemLoc;
	FRotator PlayerViewRot;
	MyWorld->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLoc, PlayerViewRot);
	InspectedItemLoc = PlayerViewLoc + (PlayerViewRot.Vector() * InspectedItemDistanceFromCam);

	FVector InspectedItemScale = SetItemScale();
	
	MoveItemToInspectionLoc(InspectedItem, ItemOriginalTransform, InspectedItemLoc, PlayerViewRot, InspectedItemScale);
}

void UCpp_InspectionComp::RestoreItemTransform(FTransform ItemOriginalTransform)
{
	if(InspectedItem)
	{
		bIsCurrentlyInspectingItem = false;
		MoveItemToOriginalLoc();
		
		if(InteractableItemIcon)
		{
			InteractableItemIcon->SetVisibility(true);
		}
	}
}

void UCpp_InspectionComp::SetInspectedItem(AActor* ItemToInspect)
{
	bItemIsInspectable = ItemToInspect->ActorHasTag("Inspectable");
	InspectedItem = ItemToInspect;
}

void UCpp_InspectionComp::RotateInspectedItem(float Amount, bool bRotateUpwards)
{
	if (bRotateUpwards)
	{
		FRotator InspectionRotation = FRotator(-Amount * .5f, 0.f, 0.f);
		InspectedItem->AddActorLocalRotation(InspectionRotation, false);
	}
	else
	{
		FRotator InspectionRotation = FRotator(0.f, -Amount * .5f, 0.f);
		InspectedItem->AddActorLocalRotation(InspectionRotation, false);
	}
}

FVector UCpp_InspectionComp::SetItemScale()
{
	ACpp_Ch_K53sc* K53sc = Cast<ACpp_Ch_K53sc>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (K53sc)
	{
		UStaticMeshComponent* StMesh = Cast<UStaticMeshComponent>(InspectedItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		float K53scRadius = K53sc->GetCapsuleComponent()->Bounds.SphereRadius;
		float InspectedItemRadius = StMesh->Bounds.SphereRadius;
		float InspectionScalePercentage = ((K53scRadius * InspectionScreenSize) / InspectedItemRadius) / 100;
		FVector FinalInspectionScale = InspectedItem->GetActorScale3D() * InspectionScalePercentage;

		return FinalInspectionScale;
	}
	return FVector(0);
}
