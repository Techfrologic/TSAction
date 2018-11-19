// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TSCharacterBase.generated.h"

class ATSProjectileWeapon;
class UAbilitySystemComponent;
class UGameplayAbility;

UENUM(BlueprintType)
enum class AbilityInput : uint8 
{
	SprintAbility UMETA(DisplayName = "Use Sprint"),
	WeaponAbility UMETA(DisplayName = "Use Weapon")
};

UCLASS()
class TSACTION_API ATSCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATSCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region  Movement Functions


	// Move along the Y axis
	void MoveUp(float value);

	// Move along the X axis
	void MoveRight(float value);

	void OnMove(float value, FVector dir);

	// Get the value returned from the y axis of the controller's left axis stick 
	void LookUp(float value);

	// Get the value returned from the x axis of the controller's left axis stick
	void LookRight(float value);

	// Rotate the character, and handle all aiming logic 
	void AimTurn();

	void SetWalkSpeed(float value);


#pragma endregion

#pragma region Action Functions
	
	// Handles sprinting input
	void OnStartSprint();

	// Handles cancel sprinting input
	void OnCancelSprint();

	void StopSprint();

	void FireWeapon();

	void StopFiring();
#pragma endregion

protected:
	

protected:

	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<UGameplayAbility> Ability;

	// The deadzone of the analog stick axes.
	float GamepadDeadZone;
	
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bIsAiming;
	
	UPROPERTY(BlueprintReadOnly, Category="Player")
	bool bIsSprinting;

	// If true, the player wanted to cancel sprint manually
	bool bManualCancelSprint;

	// Speed to move while sprinting. Overrides Character Movement walk speed
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1))
	float SprintSpeed;

	// Speed to move while moving normally. Overrides Character Movement walk speed
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1))
	float JogSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1))
	float AimWalkSpeed;

	// Rotation which the player turns while sprinting
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.f))
	float SprintRotRate;
	float DefaultRotationRate;

	/* Sets the tolerance for which the character's walk speed 
		will change while aiming, and walking backwards (the smaller the less tolerance). */
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = -1, ClampMax = 1.f))
	float AimWalkBackTolerance;
	
	// Holds the direction which is passed when the character aims
	FVector AimDirection;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ATSProjectileWeapon> StartWeapon;
	ATSProjectileWeapon* CurrentWeapon;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName ProjWeaponSocketName;

	FTimerHandle TimerHandle_OnSprint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
