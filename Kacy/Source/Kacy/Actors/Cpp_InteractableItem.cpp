// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#include "Cpp_InteractableItem.h"
#include "Classes/Components/ArrowComponent.h"
#include "Classes/Components/BoxComponent.h"
#include "Characters/K53sc/Cpp_Ch_K53sc.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
#include "Characters/K53sc/Cpp_InspectionComp.h"

ACpp_InteractableItem::ACpp_InteractableItem() :
	bIsInspectable(false),
	bIsPickupable(false),
	bIsPushable(false)
{
	PrimaryActorTick.bCanEverTick = false;

	StMesh = CreateDefaultSubobject<UStaticMeshComponent>("StMesh");
	SetRootComponent(StMesh);

	ArrowFront = CreateDefaultSubobject<UArrowComponent>("ArrowFront");
	ArrowFront->SetupAttachment(StMesh);
	BoxColFront = CreateDefaultSubobject<UBoxComponent>("BoxColFront");
	BoxColFront->SetupAttachment(ArrowFront);

	ArrowBack = CreateDefaultSubobject<UArrowComponent>("ArrowBack");
	ArrowBack->SetupAttachment(StMesh);
	BoxColBack = CreateDefaultSubobject<UBoxComponent>("BoxColBack");
	BoxColBack->SetupAttachment(ArrowBack);

	ArrowRight = CreateDefaultSubobject<UArrowComponent>("ArrowRight");
	ArrowRight->SetupAttachment(StMesh);
	BoxColRight = CreateDefaultSubobject<UBoxComponent>("BoxColRight");
	BoxColRight->SetupAttachment(ArrowRight);

	ArrowLeft = CreateDefaultSubobject<UArrowComponent>("ArrowLeft");
	ArrowLeft->SetupAttachment(StMesh);
	BoxColLeft = CreateDefaultSubobject<UBoxComponent>("BoxColLeft");
	BoxColLeft->SetupAttachment(ArrowLeft);
}

void ACpp_InteractableItem::BeginPlay()
{
	Super::BeginPlay();

	K53sc = Cast<ACpp_Ch_K53sc>(GetWorld()->GetFirstPlayerController()->GetPawn());
	InspectionComp = Cast<UCpp_InspectionComp>(K53sc->GetComponentByClass(UCpp_InspectionComp::StaticClass()));

	SetActorProperties();
	BindOverlapEvents();
}

void ACpp_InteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACpp_InteractableItem::SetActorProperties()
{
	if(bIsInspectable && !(this->Tags.Contains("Inspectable")))
	{
		this->Tags.Add("Inspectable");
	}
	else
	{
		if(!bIsInspectable)
		{
			this->Tags.Remove("Inspectable");
		}
	}

	if(bIsPickupable && !(this->Tags.Contains("Pickupable")))
	{
		this->Tags.Add("Pickupable");
	}
	else
	{
		if(!bIsPickupable)
		{
			this->Tags.Remove("Pickupable");
		}
	}

	if(bIsPushable && !(this->Tags.Contains("Pushable")))
	{
		this->Tags.Add("Pushable");
	}
	else
	{
		if(!bIsPushable)
		{
			this->Tags.Remove("Pushable");
		}
	}
}

void ACpp_InteractableItem::BindOverlapEvents()
{
	BoxColFront->SetGenerateOverlapEvents(true);
	BoxColFront->OnComponentBeginOverlap.AddDynamic(this, &ACpp_InteractableItem::OnBoxOverlapBegin);
	BoxColFront->OnComponentEndOverlap.AddDynamic(this, &ACpp_InteractableItem::OnBoxOverlapEnd);

	BoxColBack->SetGenerateOverlapEvents(true);
	BoxColBack->OnComponentBeginOverlap.AddDynamic(this, &ACpp_InteractableItem::OnBoxOverlapBegin);
	BoxColBack->OnComponentEndOverlap.AddDynamic(this, &ACpp_InteractableItem::OnBoxOverlapEnd);

	BoxColRight->SetGenerateOverlapEvents(true);
	BoxColRight->OnComponentBeginOverlap.AddDynamic(this, &ACpp_InteractableItem::OnBoxOverlapBegin);
	BoxColRight->OnComponentEndOverlap.AddDynamic(this, &ACpp_InteractableItem::OnBoxOverlapEnd);

	BoxColLeft->SetGenerateOverlapEvents(true);
	BoxColLeft->OnComponentBeginOverlap.AddDynamic(this, &ACpp_InteractableItem::OnBoxOverlapBegin);
	BoxColLeft->OnComponentEndOverlap.AddDynamic(this, &ACpp_InteractableItem::OnBoxOverlapEnd);
}

void ACpp_InteractableItem::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(InspectionComp && !InspectionComp->bIsCurrentlyInspectingItem)
	{
		if(OtherActor && OtherComp && OtherActor != this && OverlappedComp == BoxColFront)
		{
			CurrentArrow = ArrowFront;
			FVector ArrowLoc = FVector(ArrowFront->GetComponentLocation().X, ArrowFront->GetComponentLocation().Y, K53sc->GetActorLocation().Z);
			K53sc->SetActorLocation(ArrowLoc, false);
			K53sc->GetCharacterMovement()->bConstrainToPlane = true;
			K53sc->GetCharacterMovement()->SetPlaneConstraintFromVectors(ArrowFront->GetForwardVector(), ArrowFront->GetUpVector());
		}
		else if(OtherActor && OtherComp && OtherActor != this && OverlappedComp == BoxColBack)
		{
			CurrentArrow = ArrowBack;
			FVector ArrowLoc = FVector(ArrowBack->GetComponentLocation().X, ArrowBack->GetComponentLocation().Y, K53sc->GetActorLocation().Z);
			K53sc->SetActorLocation(ArrowLoc, false);
			K53sc->GetCharacterMovement()->bConstrainToPlane = true;
			K53sc->GetCharacterMovement()->SetPlaneConstraintFromVectors(ArrowBack->GetForwardVector(), ArrowBack->GetUpVector());
		}
		else if(OtherActor && OtherComp && OtherActor != this && OverlappedComp == BoxColRight)
		{
			CurrentArrow = ArrowRight;
			FVector ArrowLoc = FVector(ArrowRight->GetComponentLocation().X, ArrowRight->GetComponentLocation().Y, K53sc->GetActorLocation().Z);
			K53sc->SetActorLocation(ArrowLoc, false);
			K53sc->GetCharacterMovement()->bConstrainToPlane = true;
			K53sc->GetCharacterMovement()->SetPlaneConstraintFromVectors(ArrowRight->GetForwardVector(), ArrowRight->GetUpVector());
		}
		else if(OtherActor && OtherComp && OtherActor != this && OverlappedComp == BoxColLeft)
		{
			CurrentArrow = ArrowLeft;
			FVector ArrowLoc = FVector(ArrowLeft->GetComponentLocation().X, ArrowLeft->GetComponentLocation().Y, K53sc->GetActorLocation().Z);
			K53sc->SetActorLocation(ArrowLoc, false);
			K53sc->GetCharacterMovement()->bConstrainToPlane = true;
			K53sc->GetCharacterMovement()->SetPlaneConstraintFromVectors(ArrowLeft->GetForwardVector(), ArrowLeft->GetUpVector());
		}
	}
}

void ACpp_InteractableItem::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(InspectionComp && !InspectionComp->bIsCurrentlyInspectingItem)
	{
		if(OtherActor && OtherComp && OtherActor != this)
		{
			K53sc->GetCharacterMovement()->bConstrainToPlane = false;
		}
	}
}
