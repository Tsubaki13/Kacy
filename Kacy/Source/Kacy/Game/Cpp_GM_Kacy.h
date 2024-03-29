// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Cpp_GM_Kacy.generated.h"

class ACpp_FuseBox;

UCLASS()
class KACY_API ACpp_GM_Kacy : public AGameModeBase
{
	GENERATED_BODY()

public:
	void BeginPlay();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kacy - Fuse Box")
		ACpp_FuseBox* FuseBox;
};
