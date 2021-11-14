// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "SKTileBase.generated.h"

class UBoxComponent;
class UArrowComponent;

UCLASS(Abstract)
class SUNKENCOLONY_API ASKTileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASKTileBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// ******************************************
	// ******** Editor Exposed Properties *******
	// ******************************************
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components")
	USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components")
	UStaticMeshComponent* FloorStaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components")
	UBoxComponent* CollisionBoxEnd;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components")
	UBoxComponent* CollisionBoxBegin;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components")
	UArrowComponent* NextTileAttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components")
	UArrowComponent* PreviousTileAttachPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components")
	UArrowComponent* LaneA;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components")
	UArrowComponent* LaneB;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Components")
	UArrowComponent* LaneC;
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~ Editor Exposed Properties ~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// ******************************************
	// ************ Blueprint Events ************
	// ******************************************
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnEnteringTile(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnExitingTile(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~ Blueprint Events ~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// *****************************************
	// ********** Blueprint Callables **********
	// *****************************************
	
	UFUNCTION(BlueprintCallable)
	void HandleDestroyFloorTile();
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~ Blueprint Callables ~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
public:	
	FORCEINLINE const FTransform& GetNextTileAttachTransform() const
	{
		return NextTileAttachPoint->GetComponentTransform();
	}

private:
	UPROPERTY()
	FTimerHandle DestroyTimerHandle;
};
