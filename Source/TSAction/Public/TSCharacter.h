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

	void Aim(float value);

protected:

	UPROPERTY(VisibleAnywhere, Category="Player")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category="Player")
	USpringArmComponent* SpringArm;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
