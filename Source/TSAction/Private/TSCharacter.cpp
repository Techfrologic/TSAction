// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

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

	GamepadDeadZone = 0.25f;
}

// Called when the game starts or when spawned
void ATSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATSCharacter::MoveForward(float value)
{
	AddMovementInput(FVector(1.f,0.f,0.f) * value);
}

void ATSCharacter::MoveRight(float value)
{
	AddMovementInput(FVector(0.f,1.f,0.f) * value);
}

void ATSCharacter::LookUp(float value)
{
	SetXRotation(value);
}

void ATSCharacter::LookRight(float value)
{
	SetYRotation(value);
}

void ATSCharacter::SetXRotation(float value)
{
	WhereToLook.X = value;
	if (WhereToLook.Size() > GamepadDeadZone)
	{
		GetController()->SetControlRotation(WhereToLook.Rotation());
	}
}

void ATSCharacter::SetYRotation(float value)
{
	WhereToLook.Y = value;
	if (WhereToLook.Size() > GamepadDeadZone)
	{
		GetController()->SetControlRotation(WhereToLook.Rotation());
	}
}

// Called every frame
void ATSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement binding
	PlayerInputComponent->BindAxis("MoveForward", this, &ATSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATSCharacter::MoveRight);

	// Aiming
	PlayerInputComponent->BindAxis("LookUp", this, &ATSCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ATSCharacter::LookRight);
}

