// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSAbilityBase.h"

UTSAbilityBase::UTSAbilityBase()
{
	AbilityElements.AbilityName = FName("Ability");
	AbilityElements.bIsUnlocked = true;
	AbilityElements.KeyEvent = EInputEvent::IE_Pressed;
}

void UTSAbilityBase::Activate(AActor * ForActor)
{
	if (AbilityElements.bIsUnlocked)
	{
		OnActivated(ForActor);
	}
}

void UTSAbilityBase::SetupAbilityInput(UInputComponent * PlayerInputComponent)
{
	{
		FInputActionBinding NewAB(AbilityElements.AbilityName, AbilityElements.KeyEvent);
		FInputActionHandlerSignature NewDel;
		NewDel.BindUFunction(this, "Activate");

		NewAB.ActionDelegate = NewDel;
		PlayerInputComponent->AddActionBinding(NewAB);
	}
}