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
#include "Engine/Engine.h"
#include "TSAction.h"

#pragma region Constants
// Analog Axes names
FName R_ANALOG_Y_AXIS_NAME = FName("LookUp");
FName R_ANALOG_X_AXIS_NAME = FName("LookRight");
FName L_ANALOG_Y_AXIS_NAME = FName("MoveUp");
FName L_ANALOG_X_AXIS_NAME = FName("MoveRight");
#pragma endregion

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

	// Rotation / Aiming
	bIsAiming = false;
	AimWalkBackTolerance = -0.5f;

	bUseControllerRotationYaw = false;	// if true, causes snapping to control direction
	GetCharacterMovement()->bUseControllerDesiredRotation = true; // Allows smooth rotation
	GetCharacterMovement()->bOrientRotationToMovement = false; // Keep false; will override Aiming
	GetCharacterMovement()->RotationRate.Yaw = 1080.f;

	DesiredDirection = FVector::ZeroVector;

	// Movement
	GetCharacterMovement()->MaxAcceleration = 5000.f;
	JogSpeed = 800.f;
	SprintSpeed = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	AimWalkSpeed = JogSpeed / 2;
	bIsSprinting = false;

	// Weapon
	ProjWeaponSocketName = "ProjectileWeaponSocket";

	GamepadDeadZone = 0.25f;
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
		
		auto AttachRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,false);
		
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
	bIsAiming = (DesiredDirection.Size() > GamepadDeadZone) ? true : false;
	if (bIsAiming && !bIsSprinting)
	{
		AimTurn();
	}
	else
	{
		bIsAiming = false;
	}

	// Turn in direction of moving
	FVector AccDirection = GetCharacterMovement()->GetCurrentAcceleration();
	if (!bIsAiming && AccDirection != FVector::ZeroVector)
	{
		Turn(AccDirection);
	}

#pragma endregion

#pragma region Logs
	if (GEngine)
	{
		// Manual Cancel Sprint
		FString  ManCanSprMsg = FString::Printf(TEXT("Manual Cancel Sprint?: %s"), bManualCancelSprint ? TEXT("True") : TEXT("False"));
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green, ManCanSprMsg);

		// L analog stick X axis input
		FString LAxisX = FString::Printf(TEXT("L analog X axis input: %s"),
			*FString::SanitizeFloat(GetInputAxisValue(L_ANALOG_X_AXIS_NAME)));
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green, LAxisX);

		// L analog stick X axis input
		FString LAxisY = FString::Printf(TEXT("L analog Y axis input: %s"),
			*FString::SanitizeFloat(GetInputAxisValue(L_ANALOG_Y_AXIS_NAME)));
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green, LAxisY);

		// L analog stick XY input
		FString LAxisValMsg = FString::Printf(TEXT("L analog vect size: %s"),
			*FString::SanitizeFloat
			(
				FVector
				(
					GetInputAxisValue(L_ANALOG_X_AXIS_NAME), 
					GetInputAxisValue(L_ANALOG_Y_AXIS_NAME), 
					0.f).Size()
				)
			);
		GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Green, LAxisValMsg);

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
	if (!bIsAiming && !bIsSprinting)
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
	if (bIsAiming)
	{
		GetVelocity().Normalize();
		float dotProd = FVector::DotProduct(GetActorForwardVector(), GetVelocity());
		if (dotProd <= AimWalkBackTolerance )
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
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}
#pragma endregion 

#pragma region Action Functions

void ATSCharacter::OnStartSprint()
{
	// Cancel sprint if already sprinting
	if (bIsSprinting)
	{
		bManualCancelSprint = true;
		StopSprint();
		return;
	}

	// Increase speed if not already sprinting
	if (!bIsSprinting && !bIsAiming)
	{
		bIsSprinting = true;
		SetWalkSpeed(SprintSpeed);
	}
}

void ATSCharacter::OnCancelSprint()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_OnSprint, this, 
		&ATSCharacter::StopSprint, 0.1f, true, 0.f);
}

void ATSCharacter::StopSprint()
{
	// Cancel sprint manually
	if (bManualCancelSprint)
	{
		bIsSprinting = false;
		bManualCancelSprint = false;
		SetWalkSpeed(JogSpeed);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_OnSprint);
		return;
	}

	float yAxisValue = GetInputAxisValue(L_ANALOG_Y_AXIS_NAME);
	float xAxisValue = GetInputAxisValue(L_ANALOG_X_AXIS_NAME);
	FVector InputDir = FVector(xAxisValue, yAxisValue, 0.f);

	// Stop sprinting if already doing so and if the given left analog stick value is at the deadzone
	if (bIsSprinting)
	{
		if (InputDir.Size() < GamepadDeadZone * 2.f)
		{
			bIsSprinting = false;
			SetWalkSpeed(JogSpeed);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_OnSprint);
		}
	}
}

void ATSCharacter::FireWeapon()
{
	if (CurrentWeapon && !bIsSprinting)
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
	PlayerInputComponent->BindAxis(L_ANALOG_Y_AXIS_NAME, this, &ATSCharacter::MoveUp);
	PlayerInputComponent->BindAxis(L_ANALOG_X_AXIS_NAME, this, &ATSCharacter::MoveRight);

	// Aiming
	PlayerInputComponent->BindAxis(R_ANALOG_Y_AXIS_NAME, this, &ATSCharacter::LookUp);
	PlayerInputComponent->BindAxis(R_ANALOG_X_AXIS_NAME, this, &ATSCharacter::LookRight);

	// Actions

	// Sprinting
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ATSCharacter::OnStartSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ATSCharacter::OnCancelSprint);

	// Firing weapon
	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Pressed, this, &ATSCharacter::FireWeapon);
	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Released, this, &ATSCharacter::StopFiring);
}

