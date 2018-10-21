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

}

// Called when the game starts or when spawned
void ATSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATSCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void ATSCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void ATSCharacter::Aim(float value)
{
	AddControllerYawInput(value);
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
	PlayerInputComponent->BindAxis("Aim", this, &ATSCharacter::Aim);
}

