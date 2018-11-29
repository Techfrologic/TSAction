// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSCharacterBase.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/TSAbilityComponentBase.h"
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
ATSCharacterBase::ATSCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Rotation / Aiming
	bIsAiming = false;
	AimWalkBackTolerance = -0.5f;
	bUseControllerRotationYaw = false;	// if true, causes snapping to control direction
	GetCharacterMovement()->bUseControllerDesiredRotation = true; // Allows smooth rotation
	GetCharacterMovement()->bOrientRotationToMovement = false; // Keep false; will override Aiming
	GetCharacterMovement()->RotationRate.Yaw = 1080.f;
	AimDirection = FVector::ZeroVector;

	// Movement
	GetCharacterMovement()->MaxAcceleration = 5000.f;
	JogSpeed = 800.f;
	SprintSpeed = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	AimWalkSpeed = JogSpeed / 2;
	bIsSprinting = false;
	GamepadDeadZone = 0.25f;

	// Weapon
	ProjWeaponSocketName = "ProjectileWeaponSocket";

	// Abilities
	AbilitySystem = CreateDefaultSubobject<UTSAbilityComponentBase>(TEXT("AbilitySystem"));

}

// Called when the game starts or when spawned
void ATSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Spawn Weapon
	if (StartWeapon)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		
		auto AttachRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		
		CurrentWeapon = GetWorld()->SpawnActor<ATSProjectileWeapon>(StartWeapon, FVector::ZeroVector,
			FRotator::ZeroRotator, SpawnParams);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachRules, ProjWeaponSocketName);
	}
}


// Called every frame
void ATSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#pragma region Turn functionality

	// Turn if desired 
	bIsAiming = (AimDirection.Size() > GamepadDeadZone) ? true : false;
	if (bIsAiming && !bIsSprinting)
	{
		AimTurn();
	}
	else
	{
		bIsAiming = false;
	}

	// Turn in direction of moving
	if (!bIsAiming && GetVelocity() != FVector::ZeroVector)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

#pragma endregion

#pragma region Logs
	if (GEngine)
	{
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

void ATSCharacterBase::MoveUp(float value)
{
	FVector moveY = FVector(0.f, 1.f, 0.f);
	OnMove(value, moveY);
}

void ATSCharacterBase::MoveRight(float value)
{
	FVector moveX = FVector(1.f, 0.f, 0.f);
	OnMove(value, moveX);
}

void ATSCharacterBase::OnMove(float value, FVector dir)
{
	AddMovementInput(dir * value);
	if (!bIsAiming && !bIsSprinting)
	{
		SetWalkSpeed(JogSpeed);
	}
	
}
#pragma endregion

#pragma region Turn/Aiming Functions
void ATSCharacterBase::LookUp(float value)
{
	AimDirection.Y = value;
}

void ATSCharacterBase::LookRight(float value)
{
	AimDirection.X = value;
}

void ATSCharacterBase::AimTurn()
{	
	// Rotate based on desired aim direction
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetController()->SetControlRotation(AimDirection.Rotation());

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

void ATSCharacterBase::SetWalkSpeed(float value)
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

void ATSCharacterBase::OnStartSprint()
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

void ATSCharacterBase::OnCancelSprint()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_OnSprint, this, 
		&ATSCharacterBase::StopSprint, 0.1f, true, 0.f);
}

void ATSCharacterBase::StopSprint()
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

void ATSCharacterBase::FireWeapon()
{
	if (CurrentWeapon && !bIsSprinting)
	{
		CurrentWeapon->StartFire();
	}
}

void ATSCharacterBase::StopFiring()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

#pragma endregion

// Called to bind functionality to input
void ATSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement binding
	PlayerInputComponent->BindAxis(L_ANALOG_Y_AXIS_NAME, this, &ATSCharacterBase::MoveUp);
	PlayerInputComponent->BindAxis(L_ANALOG_X_AXIS_NAME, this, &ATSCharacterBase::MoveRight);

	// Aiming
	PlayerInputComponent->BindAxis(R_ANALOG_Y_AXIS_NAME, this, &ATSCharacterBase::LookUp);
	PlayerInputComponent->BindAxis(R_ANALOG_X_AXIS_NAME, this, &ATSCharacterBase::LookRight);

	// Actions

	// Sprinting
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ATSCharacterBase::OnStartSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ATSCharacterBase::OnCancelSprint);

	// Firing weapon
	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Pressed, this, &ATSCharacterBase::FireWeapon);
	PlayerInputComponent->BindAction("FireWeapon", EInputEvent::IE_Released, this, &ATSCharacterBase::StopFiring);

	// Ability Inputs
	AbilitySystem->SetupAbilityInputs(PlayerInputComponent);
}

