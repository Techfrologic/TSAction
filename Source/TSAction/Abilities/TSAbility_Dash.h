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
	UPROPERTY(EditDefaultsOnly, Category = "DashAbility", meta=(ClampMin = 0.f))
	float TravelDistance;

	UPROPERTY(EditDefaultsOnly, Category = "DashAbility", meta = (ClampMin = 0.f))
	float TravelSpeed;

	FTimerHandle TimerHandler_Dash;

protected:

	void OnDash();

	void Dash();

	void EndDash();

	void OnActivated_Implementation(AActor* Owner) override;

	void OnExpired_Implementation(AActor* Owner) override;
};
