// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSAbilityBase.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Public/TSAbilityComponentBase.h"

UTSAbilityBase::UTSAbilityBase()
{
	AbilityElements.AbilityName = FName("Ability");
	AbilityElements.bIsUnlocked = true;
	AbilityElements.KeyEvent = EInputEvent::IE_Pressed;
	AbilityElements.Owner = nullptr;
	AbilityElements.CurrentWorld = nullptr;
}

void UTSAbilityBase::Activate()
{
	if (AbilityElements.bIsUnlocked && GetOwner())
	{
		AbilityElements.bIsActive = true;
		OnActivated(GetOwner());
	}
}

void UTSAbilityBase::Expire()
{
	if (GetOwner())
	{
		AbilityElements.bIsActive = false;
		OnExpired(GetOwner());
	}
}

void UTSAbilityBase::SetAbilityOwner(AActor* NewOwner)
{
	if (NewOwner)
	{
		this->AbilityElements.Owner = NewOwner;
	}
}

void UTSAbilityBase::SetAbilityOwner(UTSAbilityComponentBase* AbilityComponent)
{
	if (AbilityComponent)
	{
		this->AbilityElements.Owner = AbilityComponent->GetOwner();
	}
}

void UTSAbilityBase::SetWorld(UWorld * NewWorld)
{
	if (NewWorld)
	{
		AbilityElements.CurrentWorld = NewWorld;
	}
}

AActor * UTSAbilityBase::GetOwner() const
{
	if (AbilityElements.Owner)
	{
		return AbilityElements.Owner;
	}
	return nullptr;
}

UWorld * UTSAbilityBase::GetWorld() const
{
	if (AbilityElements.CurrentWorld)
	{
		return AbilityElements.CurrentWorld;
	}
	return nullptr;
}

bool UTSAbilityBase::IsActive() const
{
	return AbilityElements.bIsActive;
}


void UTSAbilityBase::SetupAbilityInput(UInputComponent * PlayerInputComponent)
{
	// Custom input event
	{
		// Call "Activate" when the given input triggers
		FInputActionHandlerSignature NewDel;
		NewDel.BindUFunction(this, "Activate");

		FInputActionBinding NewAB(AbilityElements.AbilityName, AbilityElements.KeyEvent.GetValue());
		NewAB.ActionDelegate = NewDel;
		PlayerInputComponent->AddActionBinding(NewAB);
	}

	// Opposing input 
	{
		// Call "Expire" when the opposite of the Activate input given is released  
		FInputActionHandlerSignature NewDel;
		NewDel.BindUFunction(this, "Expire");

		// Auto correct expire input
		EInputEvent CancelInput = AbilityElements.KeyEvent.GetValue() != EInputEvent::IE_Released ? EInputEvent::IE_Pressed : EInputEvent::IE_Released;
		FInputActionBinding NewAB(AbilityElements.AbilityName, CancelInput);
		NewAB.ActionDelegate = NewDel;
		PlayerInputComponent->AddActionBinding(NewAB);
	}

}