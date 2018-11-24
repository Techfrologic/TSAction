// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSAbilityComponentBase.h"
#include "Public/TSAbilityBase.h"
#include "TSAction.h"

// Sets default values for this component's properties
UTSAbilityComponentBase::UTSAbilityComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UTSAbilityComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// Assign ability owners
	SetAbilityOwners();
}

void UTSAbilityComponentBase::SetAbilityOwners()
{
	for (auto ab : Abilities)
	{
		if (ab)
		{
			ab.GetDefaultObject()->SetAbilityOwner(GetOwner());

			UE_LOG(LogAbility, Log,TEXT("%s ability owner is %s"),
				*ab.GetDefaultObject()->GetFName().ToString(), *ab.GetDefaultObject()->GetOwner()->GetFName().ToString());
		}
	}
}

void UTSAbilityComponentBase::SetupAbilityInputs(UInputComponent * PawnInputComponent)
{
	for (auto ab : Abilities)
	{
		if (ab && PawnInputComponent)
		{
			ab.GetDefaultObject()->SetupAbilityInput(PawnInputComponent);
		}
	}
}

// Called every frame
void UTSAbilityComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}





