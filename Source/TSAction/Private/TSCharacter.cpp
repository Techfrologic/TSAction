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
	AimWalkBackTolerance = 0.5f;
	DesiredDirection = FVector::ZeroVector;

	// Movement
	GetCharacterMovement()->MaxAcceleration = 5000.f;
	JogSpeed = 800.f;
	SprintSpeed = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	AimWalkSpeed = JogSpeed / 2;
	bWantsToSprint = false;

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
		AimTurn();
	}
	else
	{
		bWantsToAim = false;
	}

	// Turn in direction of moving
	FVector AccDirection = GetCharacterMovement()->GetCurrentAcceleration();
	if (!bWantsToAim && AccDirection != FVector::ZeroVector)
	{
		Turn(AccDirection);

		/*if (bWantsToSprint)
		{
			float turnRate = 10000.f;
			FVector newRot = FMath::VInterpNormalRotationTo(GetActorLocation(), AccDirection, DeltaTime, turnRate);
			Turn(newRot);
		}
		else
		{
			Turn(AccDirection);
		}*/
	}

#pragma endregion
}

#pragma region Movement Functions

void ATSCharacter::MoveUp(float value)
{
	FVector moveY = FVector(0.f, 1.f, 0.f);
	OnMove(value, moveY);
}

void ATSCharacter::MoveRight(float value)
{
	FVector moveX = FVector(1.f, 0.f, 0.f);
	OnMove(value, moveX);
}

void ATSCharacter::OnMove(float value, FVector dir)
{
	AddMovementInput(dir * value);

	if (!bWantsToAim)
	{
		SetWalkSpeed(JogSpeed);
	}
}
#pragma endregion

#pragma region Turn/Aiming Functions
void ATSCharacter::LookUp(float value)
{
	DesiredDirection.Y = value;
}

void ATSCharacter::LookRight(float value)
{
	DesiredDirection.X = value;
}

void ATSCharacter::AimTurn()
{	
	GetController()->SetControlRotation(DesiredDirection.Rotation());

	// Slow down character if walking backwards and aiming
	if (bWantsToAim)
	{
		GetVelocity().Normalize();
		float dotProd = FVector::DotProduct(GetActorForwardVector(), GetVelocity());
		if (dotProd < AimWalkBackTolerance )
		{
			SetWalkSpeed(AimWalkSpeed);
		}
		else
		{
			SetWalkSpeed(JogSpeed);
		}
	}
}

void ATSCharacter::Turn(FVector direction)
{
	GetController()->SetControlRotation(direction.Rotation());
}

void ATSCharacter::SetWalkSpeed(float value)
{
	if (value > 0.f)
	{
		GetCharacterMovement()->MaxWalkSpeed = value;
	}
}
#pragma endregion 

#pragma region Action Functions

void ATSCharacter::StartSprint()
{
	// Increase speed if not already sprinting
	if (!bWantsToSprint && !bWantsToAim)
	{
		bWantsToSprint = true;
		SetWalkSpeed(SprintSpeed);
	}
}

void ATSCharacter::StopSprint()
{
	// Stop sprinting if already doing so.
	if (bWantsToSprint)
	{
		bWantsToSprint = false;
		SetWalkSpeed(JogSpeed);
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
	PlayerInputComponent->BindAxis("MoveUp", this, &ATSCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATSCharacter::MoveRight);

	// Aiming
	PlayerInputComponent->BindAxis(Y_AXIS_INPUT_NAME, this, &ATSCharacter::LookUp);
	PlayerInputComponent->BindAxis(X_AXIS_INPUT_NAME, this, &ATSCharacter::LookRight);

	// Actions

	// Sprinting
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ATSCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ATSCharacter::StopSprint);

	// Firing weapon
	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Pressed, this, &ATSCharacter::FireWeapon);
	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Released, this, &ATSCharacter::StopFiring);
}

