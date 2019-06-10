// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

#include "Cpp_InspectionComp.h"
#include "Cpp_Ch_K53sc.h"

// Sets default values for this component's properties
UCpp_InspectionComp::UCpp_InspectionComp() :
	InspectionTraceRange(100),
	InspectedItemDistanceFromCam(100),
	bTraceHitActor(false),
	bItemIsInspectable(false)
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
	FHitResult HitResult = InteractionTrace();
	if(InspectedItem && bItemIsInspectable)
	{
		ItemOriginalTransform = InspectedItem->GetTransform();
		SetItemInspectionTransform(HitResult);
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
	ACpp_Ch_K53sc* K53sc;
	K53sc = Cast<ACpp_Ch_K53sc>(MyWorld->GetFirstPlayerController()->GetPawn());
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

void UCpp_InspectionComp::SetItemInspectionTransform(FHitResult HitResult)
{
	FVector PlayerViewLoc, InspectedItemLoc;
	FRotator PlayerViewRot;
	MyWorld->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLoc, PlayerViewRot);
	InspectedItemLoc = PlayerViewLoc + (PlayerViewRot.Vector() * InspectedItemDistanceFromCam);

	MoveItemToInspectionLoc(InspectedItem, ItemOriginalTransform, InspectedItemLoc, PlayerViewRot);
}

void UCpp_InspectionComp::SetItemScale()
{

}

void UCpp_InspectionComp::RestoreItemTransform(FTransform ItemOriginalTransform)
{
	if(InspectedItem)
	{
		MoveItemToOriginalLoc();
	}
}
