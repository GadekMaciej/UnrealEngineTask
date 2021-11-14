// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "SKCharacter.generated.h"

class UParticleSystem;
class USoundBase;
class UAbilitySystemComponent;
class USKGameplayAbility;
class USKAbilitySystemComponent;
class USKAttributeSet;
class UGameplayEffect;

struct FOnAttributeChangeData;

/**
* Player Character base class 
* Handles basic input, uses UcharacterMovementComponent and AbilitySystemComponent
* TODO add variable to optionally disable changing lane during jump
* TODO Switch to proper Health system instead of 1 hit KO
*/ 
UCLASS(Abstract)
class SUNKENCOLONY_API ASKCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	friend USKAttributeSet;
public:	
	// Sets default values for this character's properties
	ASKCharacter();
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	// APawn interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
	// ******************************************
	// ******** Editor Exposed Properties *******
	// ******************************************
	
public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components | Camera")
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components | Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Assets")
	UParticleSystem* DeathParticleSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Assets")
	USoundBase* DeathSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Assets")
	UParticleSystem* LaneSwitchParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Assets")
	USoundBase* LaneSwitchSoundEffect;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="GAS")
	USKAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="GAS")
	USKAttributeSet* Attributes;

	// Passive Effects that will be applied to this pawn on BeginPlay()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category="GAS")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category="GAS")
	TArray<TSubclassOf<USKGameplayAbility>> GameplayAbilities;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category="Movement | Lane")
	UTimelineComponent* SwitchLaneTimeLineCPP;
	
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category="Movement | Lane")
	int32 CurrentLane = 1;
	
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category="Movement | Lane")
	int32 NextLane = 0;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~ Editor Exposed Properties ~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

public:	
	// ******************************************
	// ************ Blueprint Events ************
	// ******************************************
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Movement | Lane")
	void ChangeLane();

	UFUNCTION(BlueprintImplementableEvent, Category="Attributes")
	void OnHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Attributes")
	void OnMoveSpeedChanged(float DeltaHealth, const FGameplayTagContainer& EventTags);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Attributes")
	void OnLaneSwitchSpeedChanged(float DeltaHealth, const FGameplayTagContainer& EventTags);
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~ Blueprint Events ~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// *****************************************
	// ********** Blueprint Callables **********
	// *****************************************
	
	UFUNCTION(BlueprintCallable, Category="Movement | Lane")
	void ChangeLaneUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category="Movement | Lane")
	void ChangeLaneFinished();

	// Later on move functions should probably be merged into 1 function
	UFUNCTION(BlueprintCallable, Category="Movement")
	void MoveRight();
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	void MoveLeft();
	
	UFUNCTION(BlueprintCallable)
	void HandleDeath();
	
	UFUNCTION(BlueprintCallable)
	void HandleHitDanger();

	// Gameplay Ability System Attributes
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attributes")
	float GetHealthAttribute();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attributes")
	float GetMaxHealthAttribute();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attributes")
	float GetMoveSpeedAttribute();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attributes")
	float GetMaxMoveSpeedAttribute();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attributes")
	float GetLaneSwitchSpeedAttribute();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attributes")
	float GetMaxLaneSwitchSpeedAttribute();
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~ Blueprint Callables ~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	// *****************************************
	// ******** Gameplay Ability System ********
	// *****************************************
private:
	// Functions checking if ASC is initialized and calling appropriate events
	virtual void HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, float OverrideValue, const FGameplayTagContainer& EventTags);
	virtual void HandleLaneSwitchSpeedChanged(float DeltaHealth, const FGameplayTagContainer& EventTags);
	
	// Functions Called every time an attribute is being modified e.g by a Gameplay Effect
	virtual void OnHealthAttributeModified(const FOnAttributeChangeData& Data);
	virtual void OnMoveSpeedAttributeModified(const FOnAttributeChangeData& Data);
	virtual void OnLaneSwitchSpeedAttributeModified(const FOnAttributeChangeData& Data);

	// Initializes ASC
	void AddStartupGamePlayAbilities();

	UPROPERTY()
	uint8 bAbilitiesInitialized:1;
	
public:	
	// Epic's Interface Override
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~ Gameplay Ability System End ~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Debug")
	bool bIsDead = false;
	
	virtual void Jump() override;
	
	virtual void StopJumping() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
