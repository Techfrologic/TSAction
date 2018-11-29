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

void UTSAbility_Dash::OnDash()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandler_Dash, this,
			&UTSAbility_Dash::OnActivated, 0.f, true, 0.f);

	}
}

void UTSAbility_Dash::Dash()
{

	ATSCharacterBase* MyCharacter = Cast<ATSCharacterBase>(GetOwner());
	if (MyCharacter)
	{
		// Get the pawn's default eye location, and rotation
		FVector EyeLocation;
		FRotator EyeRotation;
		MyCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		// Draw Line to judge dash distance
		FVector DashDirection = EyeRotation.Vector();
		FVector EndLocation = EyeLocation + (EyeRotation.Vector() * TravelDistance);

		DrawDebugLine(GetWorld(), DashDirection, EndLocation, FColor::Red, false, 1.5f, 0, 2.f);

		// Dash to location
		FMath::VInterpConstantTo(DashDirection, EndLocation, GetWorld()->DeltaTimeSeconds, TravelSpeed);

		return;
	}

	UE_LOG(LogAbility, Log, TEXT("Dash failed for %s"), *GetOwner()->GetName());
}

void UTSAbility_Dash::EndDash()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler_Dash);
}

void UTSAbility_Dash::OnActivated_Implementation(AActor * Owner)
{
	OnDash();
}

void UTSAbility_Dash::OnExpired_Implementation(AActor * Owner)
{
}
