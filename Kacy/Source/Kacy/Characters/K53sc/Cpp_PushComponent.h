// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cpp_PushComponent.generated.h"

class ACpp_Ch_K53sc;
class ACpp_InteractableItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class KACY_API UCpp_PushComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCpp_PushComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Item Pushing") // this value is set in the blueprint that inherits from this class
		float WalkSpeedWhilePushing;
	
	bool bIsPushing;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	ACpp_InteractableItem* PushedItem;
	void MovePushedItem(float Amount);

private:
	// AActor* PushedItem;
	ACpp_Ch_K53sc* K53sc;
	float NormalWalkSpeed;

	UFUNCTION()
		void GrabPushable();
	UFUNCTION()
		void UngrabPushable();

	bool LookForActor(FHitResult& HitResult);
};
