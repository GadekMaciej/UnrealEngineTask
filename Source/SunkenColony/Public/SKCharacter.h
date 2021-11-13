// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "SKCharacter.generated.h"

class UParticleSystem;
class USoundBase;
class UAbilitySystemComponent;
class USKGameplayAbility;
class USKAbilitySystemComponent;
class USKAttributeSet;
class UGameplayEffect;

UCLASS(Abstract)
class SUNKENCOLONY_API ASKCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
public:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="GAS")
	USKAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS")
	USKAttributeSet* Attributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS")
	TArray<TSubclassOf<USKGameplayAbility>> GameplayAbilities;

	UPROPERTY()
	uint8 bAbilitiesInitialized:1;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category="Movement | Lane")
	int32 CurrentLane = 1;
	
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category="Movement | Lane")
	int32 NextLane = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement | Lane")
	float DefaultLaneSwitchSpeedMultiplier = 1.0f;
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite, Category="Movement | Lane")
	float CurrentLaneSwitchSpeedMultiplier = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Debug")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	UParticleSystem* DeathParticleSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	USoundBase* DeathSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	UParticleSystem* LaneSwitchParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	USoundBase* LaneSwitchSoundEffect;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Movement | Lane")
	void ChangeLane();

	UFUNCTION(BlueprintCallable, Category="Movement | Lane")
	void ChangeLaneUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category="Movement | Lane")
	void ChangeLaneFinished();

	// Later on move functions should probably be merged into 1 function
	UFUNCTION(BlueprintCallable, Category="Movement")
	void MoveRight();
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	void MoveLeft();

	void AddStartupGamePlayAbilities();
	
	virtual void Jump() override;
	
	virtual void StopJumping() override;

	UFUNCTION(BlueprintCallable)
	void HandleDeath();
	
	UFUNCTION(BlueprintCallable)
	void HandleHitDanger();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaHealth, const FGameplayTagContainer& EventTags);

	virtual void HandleHealthChanged(float DeltaHealth, const FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaHealth, const FGameplayTagContainer& EventTags);

	virtual void HandleMoveSpeedChanged(float DeltaHealth, const FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLaneSwitchSpeedChanged(float DeltaHealth, const FGameplayTagContainer& EventTags);

	virtual void HandleLaneSwitchSpeedChanged(float DeltaHealth, const FGameplayTagContainer& EventTags);

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
	
	friend USKAttributeSet;

	// Sets default values for this character's properties
	ASKCharacter();
	
	protected:
	// APawn interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
