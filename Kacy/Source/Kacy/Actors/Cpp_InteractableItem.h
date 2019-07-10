// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_InteractableItem.generated.h"

class ACpp_Ch_K53sc;
class UArrowComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UCpp_InspectionComp;
class UWidgetComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UStaticMeshComponent* StMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UArrowComponent* ArrowFront;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UArrowComponent* ArrowBack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UArrowComponent* ArrowRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UArrowComponent* ArrowLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UBoxComponent* BoxColFront;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UBoxComponent* BoxColBack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UBoxComponent* BoxColRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UBoxComponent* BoxColLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kacy - Basic Components")
		UWidgetComponent* IconWidget;

	UArrowComponent* CurrentArrow;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	ACpp_Ch_K53sc* K53sc;

	void SetComponentsProperties();
	void SetActorProperties();
	void BindOverlapEvents();
	void SetIconRotation();
	void SetIconSize();

	UFUNCTION()
		void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UCpp_InspectionComp* InspectionComp;

	FVector  IconCurrentLoc, PlayerCamLoc;
	FRotator PlayerCamRot;
};
