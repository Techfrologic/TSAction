// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSProjectileWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ATSProjectileWeapon::ATSProjectileWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = Mesh;

	MuzzleSocketName = "MuzzleSocket";
}

// Called when the game starts or when spawned
void ATSProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATSProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

