// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSAbilityComponentBase.h"
#include "Public/TSAbilityBase.h"

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

	// ...
}

void UTSAbilityComponentBase::SetupAbilityInputs(UInputComponent * PawnInputComponent)
{
	for (auto ab : Abilities)
	{
		if (ab && PawnInputComponent)
		{
			ab.GetDefaultObject()->SetupAbilityInput(PawnInputComponent);

			//UTSAbilityBase* Ability = ab.GetDefaultObject();
			//PawnInputComponent->BindAction(
			//	Ability->AbilityElements.AbilityName, Ability->AbilityElements.KeyEvent,
			//	GetOwner(), &GetOwner():);
		}
		
	}
}

// Called every frame
void UTSAbilityComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}





