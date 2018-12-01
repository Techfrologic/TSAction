// Fill out your copyright notice in the Description page of Project Settings.

#include "TSAbility_Dash.h"
#include "Public/TSCharacterBase.h"
#include "TSAction.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UTSAbility_Dash::UTSAbility_Dash()
{
	AbilityElements.AbilityName = "Dash";
	TravelDistance = 500.f;
	TravelSpeed = 500.f;
}
