// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TSProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Public/TSCharacter.h"

// Sets default values
ATSProjectile::ATSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh setup setup
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ATSProjectile::HandleOnOverlap);
	RootComponent = Mesh;

	Mesh->SetSimulatePhysics(true);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	BaseDamage = 20.f;

	LifeSpan = 3.f;
}

// Called when the game starts or when spawned
void ATSProjectile::BeginPlay()
{
	Super::BeginPlay();

	Mesh->IgnoreActorWhenMoving(this->GetOwner()->Instigator, true);
	SetLifeSpan(LifeSpan);
}

void ATSProjectile::HandleOnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Apply damage to overlapped character that's not this owner
	ATSCharacter* Enemy = Cast<ATSCharacter>(OtherActor);
	if (Enemy && Enemy != this->GetOwner())
	{
		UGameplayStatics::ApplyDamage(Enemy,BaseDamage,this->GetInstigatorController(), this, NULL);
		this->Destroy();
	}
}

// Called every frame
void ATSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

