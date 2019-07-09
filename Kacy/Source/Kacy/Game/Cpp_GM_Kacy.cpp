// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#include "Cpp_GM_Kacy.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Actors/Cpp_FuseBox.h"

void ACpp_GM_Kacy::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundFuseBoxes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACpp_FuseBox::StaticClass(), FoundFuseBoxes);

	if (FoundFuseBoxes.Num() > 0)
	{
		FuseBox = Cast<ACpp_FuseBox>(FoundFuseBoxes[0]);
	}
}
