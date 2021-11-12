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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* FloorStaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* CollisionBoxEnd;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* CollisionBoxBegin;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* NextTileAttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* PreviousTileAttachPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* LaneA;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* LaneB;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* LaneC;

	UPROPERTY()
	FTimerHandle DestroyTimerHandle;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnEnteringTile(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintNativeEvent)
	void OnExitingTile(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void HandleDestroyFloorTile();
	
	FORCEINLINE const FTransform& GetNextTileAttachTransform() const
	{
		return NextTileAttachPoint->GetComponentTransform();
	}
	
protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
