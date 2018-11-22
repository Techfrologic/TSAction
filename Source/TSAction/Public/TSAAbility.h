// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TSAAbility.generated.h"

class AActor;

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UTSAAbility : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TSACTION_API ITSAAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	ITSAAbility();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Abilities")
	void Activate(AActor* ForActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Abilities")
	void OnActivated(AActor* ForActor);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Abilities")
	void OnExpired();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Abilities")
	void SetupAbilityInput(class UInputComponent* PlayerInputComponent);
};
