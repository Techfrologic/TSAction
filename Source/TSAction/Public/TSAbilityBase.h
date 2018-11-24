// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/TSAAbilityInterface.h"
#include "UObject/NoExportTypes.h"
#include "TSAbilityBase.generated.h"


class UTSAbilityComponent;

// Elements which make up ability functionality
USTRUCT(Blueprintable)
struct TSACTION_API FAbilityElements
{
	GENERATED_BODY()

	// The name of the ability
	UPROPERTY(EditDefaultsOnly, Category = "AbilityElements")
	FName AbilityName;

	// If true, allows the ability to be activated
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilityElements")
	bool bIsUnlocked;

	// The type of input used to activate ability
	UPROPERTY(EditDefaultsOnly, Category = "AbilityElements")
	TEnumAsByte<EInputEvent> KeyEvent;

	// This Ability's owner
	AActor* Owner;

	bool bIsActive;
};
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TSACTION_API UTSAbilityBase : public UObject, public ITSAbilityInterface
{
	GENERATED_BODY()

	friend class UTSAbilityComponentBase;
	
public:
	UTSAbilityBase();

	UFUNCTION()
	void Activate() override;

	UFUNCTION()
	void Expire() override;

	void SetAbilityOwner(AActor* NewOwner);

	void SetAbilityOwner(UTSAbilityComponentBase* AbilityComponent);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	AActor* GetOwner() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool IsActive() const;

protected:
	// Handles functionality when ability is activated.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
	void OnActivated(AActor* Owner);

	// Handles functionality when ability component has expired.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
	void OnExpired(AActor* Owner);

	/**<summary>Binds this ability's functionality</summary>
		<param>PlayerInputComponent - The Actor InputComponent who will be using the ability</param>
		@return - new binding action of this type, based on the ability's name.
	*/
	virtual void SetupAbilityInput(class UInputComponent* PlayerInputComponent) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	FAbilityElements AbilityElements;

	FTimerHandle TimerHandle_ActivateAbility;
};
