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


	// Move along the Y axis
	void MoveUp(float value);

	// Move along the X axis
	void MoveRight(float value);

	void OnMove(float value, FVector dir);

	// Get the value returned from the y axis of the controller's left axis stick 
	void LookUp(float value);

	// Get the value returned from the x axis of the controller's left axis stick
	void LookRight(float value);

	// Rotate the character, and handle all aiming logic 
	void AimTurn();

	void SetWalkSpeed(float value);


#pragma endregion

#pragma region Action Functions
	
	// Handles sprinting input
	void OnStartSprint();

	// Handles cancel sprinting input
	void OnCancelSprint();

	void StopSprint();

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
	
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bIsAiming;
	
	UPROPERTY(BlueprintReadOnly, Category="Player")
	bool bIsSprinting;

	// If true, the player wanted to cancel sprint manually
	bool bManualCancelSprint;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1))
	float SprintSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1))
	float JogSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1))
	float AimWalkSpeed;

	// Rotation which the player turns while sprinting
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.f))
	float SprintRotRate;

	float DefaultRotationRate;

	/* Sets the tolerance for which the character's walk speed 
		will change while aiming, and walking backwards (the smaller the less tolerance). */
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = -1, ClampMax = 1.f))
	float AimWalkBackTolerance;
	
	// Holds the direction which is passed when the character aims
	FVector AimDirection;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ATSProjectileWeapon> StartWeapon;

	ATSProjectileWeapon* CurrentWeapon;

	FTimerHandle TimerHandle_OnSprint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName ProjWeaponSocketName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
