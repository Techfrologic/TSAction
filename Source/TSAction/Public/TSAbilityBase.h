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

	UWorld* CurrentWorld;

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
	virtual void Activate();
	
	UFUNCTION()
	virtual void Expire();

	void SetAbilityOwner(AActor* NewOwner);

	void SetAbilityOwner(UTSAbilityComponentBase* AbilityComponent);

	void SetWorld(UWorld* NewWorld);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	AActor* GetOwner() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	UWorld* GetWorld() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool IsActive() const;

protected:
	// Handles functionality when ability is activated.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void OnActivated(AActor* Owner) override;
	virtual void OnActivated_Implementation(AActor* Owner) { return; };


	// Handles functionality when ability component has expired.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void OnExpired(AActor* Owner) override;
	virtual void OnExpired_Implementation(AActor* Owner) { return; };

	/**<summary>Binds this ability's functionality</summary>
		@PlayerInputComponent - The Pawn's InputComponent who will be using the ability
		@return - new binding action of this type, based on the ability's name.
	*/
	virtual void SetupAbilityInput(class UInputComponent* PlayerInputComponent) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	FAbilityElements AbilityElements;
};
