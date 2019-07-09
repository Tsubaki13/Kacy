// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_FuseBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPowerDelegate);

UCLASS()
class KACY_API ACpp_FuseBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ACpp_FuseBox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "Kacy - Fuse Box")
		FPowerDelegate OnPowerOn;
};
