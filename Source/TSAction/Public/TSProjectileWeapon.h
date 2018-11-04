// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSProjectileWeapon.generated.h"

class USkeletalMeshComponent;
class ATSProjectile;

UCLASS()
class TSACTION_API ATSProjectileWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATSProjectileWeapon();

	// Sets up timer calling Fire()
	void StartFire();

	// Clears timer calling fire
	void StopFire();

protected:

	// Spawn bullet type to fire
	void Fire();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ATSProjectile> BulletType;

	// Handles firing ticks
	FTimerHandle TimerHandler_TimeBetweenShots;

	float LastShotFired;
	
	float TimeBetweenShots;

	float LastFireTime;

	// Number of seconds weapon can fire a projectile 
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta=(ClampMin = 0.f))
	float RateOfFire;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
