// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/TSAAbilityInterface.h"
#include "TSAbilityComponentBase.generated.h"

// Elements which make up ability functionality
USTRUCT()
struct TSACTION_API FAbilityElements
{
	GENERATED_BODY()

	// The name of the ability

	UPROPERTY(EditDefaultsOnly, Category = "AbilityElements")
	FName AbilityName;

	// If true, allows the ability to be activated
	UPROPERTY(EditDefaultsOnly, Category = "AbilityElements")
	bool bIsUnlocked;

	// The type of input used to activate ability
	UPROPERTY(EditDefaultsOnly, Category = "AbilityElements")
	TEnumAsByte<EInputEvent> KeyEvent;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class TSACTION_API UTSAbilityComponentBase : public UActorComponent, public ITSAbilityInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTSAbilityComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Activate(AActor* ForActor) override;

	// Handles functionality when ability component is activated. Is a BlueprintNativeEvent
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
	void OnActivated(AActor* ForActor);

	// Handles functionality when ability component has expired. Is a BlueprintNativeEvent
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
	void OnExpired();

	/**<summary>Binds this ability's functionality</summary>
		<param>PlayerInputComponent - The Actor InputComponent who will be using the ability</param>
		@return - new binding action of this type, based on the ability's name.
	*/
	virtual void SetupAbilityInput(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	FAbilityElements AbilityElements;
};
