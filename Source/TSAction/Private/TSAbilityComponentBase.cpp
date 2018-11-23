// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSAbilityComponentBase.h"


// Sets default values for this component's properties
UTSAbilityComponentBase::UTSAbilityComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AbilityElements.AbilityName = FName("Ability");
	AbilityElements.bIsUnlocked = true;
	AbilityElements.KeyEvent = EInputEvent::IE_Pressed;
}


// Called when the game starts
void UTSAbilityComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UTSAbilityComponentBase::Activate(AActor * ForActor)
{
	if (AbilityElements.bIsUnlocked)
	{
		OnActivated(ForActor);
	}
}


// Called every frame
void UTSAbilityComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTSAbilityComponentBase::SetupAbilityInput(UInputComponent * PlayerInputComponent)
{
	FInputActionBinding NewAB;
	NewAB.KeyEvent = AbilityElements.KeyEvent.GetValue();

	FInputActionHandlerSignature NewDel;
	NewDel.BindUFunction(this, AbilityElements.AbilityName);

	NewAB.ActionDelegate = NewDel;
	PlayerInputComponent->AddActionBinding(NewAB);
}



