// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/TSAbilityBase.h"
#include "TSAbility_Dash.generated.h"

/**
 * 
 */
UCLASS()
class TSACTION_API UTSAbility_Dash : public UTSAbilityBase
{
	GENERATED_BODY()

public:
	UTSAbility_Dash();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DashMovement", meta=(ClampMin = 0.f))
	float TravelDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DashMovement", meta = (ClampMin = 0.f))
	float TravelSpeed;
};
