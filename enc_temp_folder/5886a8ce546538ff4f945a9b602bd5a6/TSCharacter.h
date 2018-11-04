// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TSCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ATSProjectileWeapon;

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

#pragma region  Movement Functions



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

	void Turn(float direction);

#pragma endregion

#pragma region Action Functions

	void StartSprint();

	void StopSprint();

	/// TODO: FIX LATER
	/** <summary>Moves the character at a desired input value</summary>
		@inputVal - The input value which to move the character
		@dir - direction to move in*/
	void OnMove(float inputVal, FVector dir);

	/** <summary>Character sprints at a desired input value</summary>
		@inputVal - The input value which to move the character
		@storedDir - Stored direction of character.
		@dir - direction to sprint
		@turnResist - if true, resists between the inputVal, and the storedDir*/
	void OnSprint(float inputVal, float storedDir, FVector dir, bool turnResist);
	///

	void FireWeapon();

	void StopFiring();
#pragma endregion


protected:

	UPROPERTY(VisibleAnywhere, Category="Player")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category="Player")
	USpringArmComponent* SpringArm;

	// The deadzone of the analog stick axes.
	float GamepadDeadZone;
	
	bool bWantsToAim;
	
	UPROPERTY(BlueprintReadOnly, Category="Player")
	bool bWantsToSprint;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float SprintSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float JogSpeed;

	// Resistance while sprinting
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta=(ClampMin= 0.1))
	float SprintTurnResistRate;

	// If true, adds turn resistance when sprinting. 
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	bool bSprintTurnResist;

	FVector DesiredDirection;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ATSProjectileWeapon> StartWeapon;

	ATSProjectileWeapon* CurrentWeapon;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName ProjWeaponSocketName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
