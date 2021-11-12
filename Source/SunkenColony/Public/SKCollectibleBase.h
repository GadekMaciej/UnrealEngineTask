// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SKCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "SKCollectibleBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UShphereComponent;
class URotatingMovementComponent;
class USoundBase;

// Base class for any sort of power ups and coins
UCLASS(Abstract)
class SUNKENCOLONY_API ASKCollectibleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASKCollectibleBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	URotatingMovementComponent* RotatingMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Assets")
	USoundBase* OnCollectSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="settings")
	int32 ScoreValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float HeightDifferenceRequiredForJumpedOn = 50.0f;
	
	// called when player collects this collectible
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Events")
	void OnCollected(ASKCharacter* PlayerCharacter);
	// called when player jumps on top of this collectible
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Events")
	void OnJumpedOnTop(ASKCharacter* PlayerCharacter);
	
	// calls OnCollected() Event;
	UFUNCTION()
	void OnSphereColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

public:	

};
