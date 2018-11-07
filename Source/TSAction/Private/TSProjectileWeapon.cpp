// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSProjectileWeapon.h"
#include "Public/TSCharacter.h"
#include "Public/TSProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"

// Sets default values
ATSProjectileWeapon::ATSProjectileWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = Mesh;

	MuzzleSocketName = "MuzzleSocket";

	// Firing
	RateOfFire = 500.f;
}

void ATSProjectileWeapon::Fire()
{
	// Spawn bullet at the muzzle socket
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Cast<ATSCharacter>(this->GetOwner());

	if (Mesh != NULL && BulletType != NULL)
	{
		FVector SocketLocation = Mesh->GetSocketLocation(MuzzleSocketName);
		FRotator SocketRotation = Mesh->GetSocketRotation(MuzzleSocketName);

		GetWorld()->SpawnActor<ATSProjectile>(BulletType.Get(), SocketLocation, SocketRotation, SpawnParams);
	}
}

void ATSProjectileWeapon::StartFire()
{
	// Getting the greater value between the calculated delay and 0.
	float firstDelay = FMath::Max((LastFireTime + TimeBetweenShots) - GetWorld()->TimeSeconds, 0.f);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandler_TimeBetweenShots, this,
		&ATSProjectileWeapon::Fire, TimeBetweenShots, true, firstDelay);
}

void ATSProjectileWeapon::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler_TimeBetweenShots);
	LastFireTime += FiringBuffer;
}

// Called when the game starts or when spawned
void ATSProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// Set time needed between shots
	TimeBetweenShots = 60 / RateOfFire;
	LastFireTime = -TimeBetweenShots;
}

// Called every frame
void ATSProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

