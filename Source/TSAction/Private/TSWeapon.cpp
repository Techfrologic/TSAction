// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ATSWeapon::ATSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void ATSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

