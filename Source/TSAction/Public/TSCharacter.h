// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TSCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TSACTION_API ATSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	// Get the value returned from the y axis of the controller
	void LookUp(float value);

	// Get the value returned from the x axis of the controller
	void LookRight(float value);

	/*  
		Rotate character to the desired direction. 
		This is based on the desired direction given from player input
	*/
	void Turn();

	// Rotate character to the desired direction based on a given direction
	void Turn(FVector direction);

protected:

	UPROPERTY(VisibleAnywhere, Category="Player")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category="Player")
	USpringArmComponent* SpringArm;

	// The deadzone of the analog stick axes.
	float GamepadDeadZone;
	bool bWantsToAim;
	FVector DesiredDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
