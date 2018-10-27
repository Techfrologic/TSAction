// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TSAction.h"

// Sets default values
ATSCharacter::ATSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera Component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArm);

	// Turning;
	bWantsToAim = false;
	GamepadDeadZone = 0.25f;
	DesiredDirection = FVector::ZeroVector;
	secondsToTurn = 0.3;

	// Movement
	GetCharacterMovement()->MaxAcceleration = 5000.f;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
}

// Called when the game starts or when spawned
void ATSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATSCharacter::MoveForward(float value)
{
	AddMovementInput(FVector(0.f,1.f,0.f) * value);
}

void ATSCharacter::MoveRight(float value)
{
	AddMovementInput(FVector(1.f,0.f,0.f) * value);
}

void ATSCharacter::LookUp(float value)
{
	DesiredDirection.Y = value;
}

void ATSCharacter::LookRight(float value)
{
	DesiredDirection.X = value;
}

void ATSCharacter::Turn()
{	
	GetController()->SetControlRotation(DesiredDirection.Rotation());
}

void ATSCharacter::Turn(FVector direction)
{
	GetController()->SetControlRotation(direction.Rotation());
}

void ATSCharacter::Turn(float direction)
{
	GetController()->SetControlRotation(FRotator(0,direction,0));
}

// Called every frame
void ATSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
#pragma region Turn functionality

	// Turn if desired 
	bWantsToAim = (DesiredDirection.Size() > GamepadDeadZone) ? true : false;

	// TODO: Smoothly turn to the new direction
	// FVector newDir = FMath::VInterpTo(GetActorLocation(), CurrentAcceleration, DeltaTime, secondsToTurn);
	if (bWantsToAim)
	{
		Turn();
		// Turn results
		UE_LOG(LogTemp, Log, TEXT("Turn Results: %s, "), *DesiredDirection.ToString());
		UE_LOG(LogTemp, Log, TEXT("Wants to Aim?: %s, "), (this->bWantsToAim ? TEXT("TRUE") : TEXT("FALSE")));
	}
	else
	{
		bWantsToAim = false;
	}

	// Turn in direction of acceleration if moving
	FVector CurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();
	if (!bWantsToAim && CurrentAcceleration != FVector::ZeroVector)
	{
		Turn(CurrentAcceleration);
	}
#pragma endregion

	
}

// Called to bind functionality to input
void ATSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement binding
	PlayerInputComponent->BindAxis("MoveForward", this, &ATSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATSCharacter::MoveRight);

	// Aiming
	PlayerInputComponent->BindAxis(Y_AXIS_INPUT_NAME, this, &ATSCharacter::LookUp);
	PlayerInputComponent->BindAxis(X_AXIS_INPUT_NAME, this, &ATSCharacter::LookRight);
}

