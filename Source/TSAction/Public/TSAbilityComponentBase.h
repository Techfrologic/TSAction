// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TSAbilityComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class TSACTION_API UTSAbilityComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTSAbilityComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Assign the owner of each ability 
	void SetAbilityOwners();

protected:

	// List of abilities for this component
	UPROPERTY(EditDefaultsOnly, Category= "Abilities")
	TArray<TSubclassOf<class UTSAbilityBase>> Abilities;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupAbilityInputs(class UInputComponent* PawnInputComponent);
};
