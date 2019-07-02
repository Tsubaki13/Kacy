// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_InteractableItem.generated.h"

UCLASS()
class KACY_API ACpp_InteractableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ACpp_InteractableItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Interactable") // this value is set in the blueprint that inherits from this class
		bool bIsInspectable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Interactable") // this value is set in the blueprint that inherits from this class
		bool bIsPickupable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Interactable") // this value is set in the blueprint that inherits from this class
		bool bIsPushable;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void SetActorProperties();
};
