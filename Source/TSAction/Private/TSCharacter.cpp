// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/TSProjectileWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
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

	// Movement
	GetCharacterMovement()->MaxAcceleration = 5000.f;
	JogSpeed = 800.f;
	SprintSpeed = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	AimWalkSpeed = JogSpeed / 2;
	bWantsToSprint = false;
	SprintTurnResistRate = 10000.f;
	bSprintTurnResist = true;

	// Weapon
	ProjWeaponSocketName = "ProjectileWeaponSocket";

	
}

// Called when the game starts or when spawned
void ATSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Spawn Weapon
	if (StartWeapon)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget,false);

		CurrentWeapon = GetWorld()->SpawnActor<ATSProjectileWeapon>(StartWeapon, FVector::ZeroVector, 
			FRotator::ZeroRotator, SpawnParams);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachRules, ProjWeaponSocketName);
	}
}

// Called every frame
void ATSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#pragma region Turn functionality

	// Turn if desired 
	bWantsToAim = (DesiredDirection.Size() > GamepadDeadZone) ? true : false;
	if (bWantsToAim && !bWantsToSprint)
	{
		Turn();
	}
	else
	{
		bWantsToAim = false;
	}

	// Turn in direction of moving
	FVector AccDirection = GetCharacterMovement()->GetCurrentAcceleration();
	if (!bWantsToAim && AccDirection != FVector::ZeroVector)
	{
		if (bWantsToSprint)
		{
			FVector newRot = FMath::VInterpNormalRotationTo(GetActorLocation(), AccDirection, DeltaTime, SprintTurnResistRate);
			Turn(newRot);
		}
		else
		{
			Turn(AccDirection);
		}
	}

#pragma endregion


}

/*** TODO: Interpolate the value param to slow down X & Y Turning ***/
#pragma region Movement Functions

void ATSCharacter::MoveForward(float value)
{
	FVector moveY = FVector(0.f, 1.f, 0.f);
	OnMove(value, moveY);
}

void ATSCharacter::MoveRight(float value)
{
	FVector moveX = FVector(1.f, 0.f, 0.f);
	OnMove(value, moveX);
}

void ATSCharacter::LookUp(float value)
{
	DesiredDirection.Y = value;
}

void ATSCharacter::LookRight(float value)
{
	DesiredDirection.X = value;
}

void ATSCharacter::OnMove(float inputVal, FVector dir)
{
	AddMovementInput(dir * inputVal);
}

void ATSCharacter::OnSprint(float inputVal, float storedDir, FVector dir, bool turnResist)
{
	if (turnResist)
	{
		if (inputVal >= storedDir - SprintTurnResistRate
			&& inputVal <= storedDir + SprintTurnResistRate)
		{
			AddMovementInput(dir * inputVal);
		}
	}
	else
	{
		AddMovementInput(dir * inputVal);
	}
}

void ATSCharacter::Turn()
{	
	GetController()->SetControlRotation(DesiredDirection.Rotation());

	if (bWantsToAim)
	{
		GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
	}
}

void ATSCharacter::Turn(FVector direction)
{
	GetController()->SetControlRotation(direction.Rotation());

	if (bWantsToAim)
	{
		GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
	}
}
#pragma endregion

#pragma region Action Functions
void ATSCharacter::StartSprint()
{
	if (!bWantsToSprint && !bWantsToAim)
	{
		bWantsToSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ATSCharacter::StopSprint()
{
	if (bWantsToSprint)
	{
		bWantsToSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
}

void ATSCharacter::FireWeapon()
{
	if (CurrentWeapon && !bWantsToSprint)
	{
		CurrentWeapon->StartFire();
	}
}

void ATSCharacter::StopFiring()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

#pragma endregion



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

	// Actions

	// Sprinting
	PlayerInputComponent->BindAction("StartSprint", EInputEvent::IE_Pressed, this, &ATSCharacter::StartSprint);
	PlayerInputComponent->BindAction("StopSprint", EInputEvent::IE_Released, this, &ATSCharacter::StopSprint);

	// Firing weapon
	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Pressed, this, &ATSCharacter::FireWeapon);
	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Released, this, &ATSCharacter::StopFiring);
}

