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

	void StartSprint();

	void StopSprint();

	/** <summary>Moves the character at a desired input value</summary>
		@inputVal - The input value which to move the character
		@storeVal - stores the inputVal into a specified variable. Used for sprinting 
		@dir - direction to move in
		@sprintEnabled - if true, stores inputVal into storeVal*/
	void OnMove(float inputVal, float storeVal, FVector dir, bool sprintEnabled = true);

	/** <summary>Character sprints at a desired input value</summary>
		@inputVal - The input value which to move the character
		@storedDir - Stored direction of character. 
		@dir - direction to sprint
		@turnResist - if true, resists between the inputVal, and the storedDir*/
	void OnSprint(float inputVal, float storedDir, FVector dir, bool turnResist = true);

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

	float sprintDirY;

	float sprintDirX;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	bool TurnResist;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float JogSpeed;

	// Resistance while sprinting
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta=(ClampMin= 0.1))
	float SprintTurnResist;

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
