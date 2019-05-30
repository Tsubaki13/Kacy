// This game was designed and developed by OURTEAMNAME as the BA2 semester project for the summer semester 2019. OURTEAMNAME's members: Jann Albrecht (Designer). Patrick Handwerk (Programmer). Mohammed Najeeb Mshaweh (Programmer). Bjoern Roethig (3D Artist). Pauline Mueller (2D Artist).

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CppAnim_K53sc.generated.h"


UCLASS()
class ISTHISLOVE_API UCppAnim_K53sc : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationVariables")
		float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationVariables")
		bool bIsFalling;
};
