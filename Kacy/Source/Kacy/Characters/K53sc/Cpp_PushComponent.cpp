// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).


#include "Cpp_PushComponent.h"
#include "Characters/K53sc/Cpp_Ch_K53sc.h"
#include "Actors/Cpp_InteractableItem.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
#include "Classes/Components//SkeletalMeshComponent.h"

UCpp_PushComponent::UCpp_PushComponent() :
	bIsPushing(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCpp_PushComponent::BeginPlay()
{
	Super::BeginPlay();

	K53sc = Cast<ACpp_Ch_K53sc>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	NormalWalkSpeed = K53sc->GetCharacterMovement()->GetMaxSpeed();
	
	K53sc->OnGrabItem.AddDynamic(this, &UCpp_PushComponent::GrabPushable);
	K53sc->OnUngrabItem.AddDynamic(this, &UCpp_PushComponent::UngrabPushable);
}

void UCpp_PushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCpp_PushComponent::GrabPushable()
{
	FHitResult HitResult;

	if(LookForActor(HitResult))
	{
		if(HitResult.GetActor()->ActorHasTag("Pushable"))
		{
			ItemHit = Cast<ACpp_InteractableItem>(HitResult.GetActor());

			if(ItemHit->bIsPushable)
			{
				USkeletalMeshComponent* SkMesh = Cast<USkeletalMeshComponent>(K53sc->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

				K53sc->GetCharacterMovement()->MaxWalkSpeed = WalkSpeedWhenPushing;
				FAttachmentTransformRules AttachRules = AttachRules.KeepWorldTransform;
				ItemHit->AttachToComponent(SkMesh, AttachRules, "RootSocket");
				bIsPushing = true;
			}
		}
	}
}

void UCpp_PushComponent::UngrabPushable()
{
	if (ItemHit)
	{
		K53sc->GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		FDetachmentTransformRules DetachRules = DetachRules.KeepWorldTransform;
		ItemHit->DetachFromActor(DetachRules);
		ItemHit = nullptr;
		bIsPushing = false;
	}
}

bool UCpp_PushComponent::LookForActor(FHitResult& HitResult)
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	FVector TraceEnd = PlayerViewLocation + (PlayerViewRotation.Vector() * 200);
	FCollisionQueryParams ColParams;
	ColParams.AddIgnoredActor(K53sc);

	return GetWorld()->LineTraceSingleByChannel(HitResult, PlayerViewLocation, TraceEnd, ECC_WorldDynamic, ColParams);
}
