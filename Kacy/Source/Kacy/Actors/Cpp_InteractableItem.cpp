// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#include "Cpp_InteractableItem.h"

ACpp_InteractableItem::ACpp_InteractableItem() :
	bIsInspectable(false)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACpp_InteractableItem::BeginPlay()
{
	Super::BeginPlay();

	if(bIsInspectable && !(this->Tags.Num() > 0))
	{
			this->Tags.Add("Inspectable");
	}
	else
	{
		if(!bIsInspectable)
		{
			this->Tags.Empty();
		}
	}
}

void ACpp_InteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
