// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	URotatingMovementComponent* RotatingMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Assets")
	USoundBase* OnCollectSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	float HeightDifferenceRequiredForJumpedOn = 50.0f;

	// called when player collects this collectible
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Events")
	void OnCollected();
	// called when player jumps on top of this collectible
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Events")
	void OnJumpedOnTop();
	
	// calls OnCollected() Event;
	UFUNCTION()
	void OnSphereColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

public:	

};
