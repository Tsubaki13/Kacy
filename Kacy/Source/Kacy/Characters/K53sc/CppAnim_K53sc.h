// This game was designed and developed by team OnTime as the BA2 semester project for the summer semester 2019 in CologneGameLab in Cologne, Germany. Team OnTime's members: Jann Albrecht (Designer), Patrick Handwerk (Programmer), Mohammed Najeeb Mshaweh (Programmer), Bjoern Roethig (3D Artist), Sameh Aransa (3D Artist), and Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CppAnim_K53sc.generated.h"


UCLASS()
class KACY_API UCppAnim_K53sc : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationVariables")
		FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationVariables")
		bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationVariables")
		bool bIsInspecting;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationVariables")
		bool bIsPushing;
};
