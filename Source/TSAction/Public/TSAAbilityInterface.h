// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/InputComponent.h"
#include "TSAAbilityInterface.generated.h"

class AActor;

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UTSAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TSACTION_API ITSAbilityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	UFUNCTION()
	virtual void Activate()=0;

	UFUNCTION()
		virtual void Expire() = 0;
protected:
	virtual void SetupAbilityInput(class UInputComponent* PlayerInputComponent) =0;
};
