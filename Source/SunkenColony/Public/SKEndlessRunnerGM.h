// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SKEndlessRunnerGM.generated.h"

class ASKTileBase;
/**
 * 
 */
UCLASS(Abstract)
class SUNKENCOLONY_API ASKEndlessRunnerGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASKEndlessRunnerGM();
	
	UPROPERTY(EditDefaultsOnly, Category="EndlessRunner | Tiles")
	int32 NumOfInitFloorTiles = 8;
	
	UPROPERTY(EditDefaultsOnly, Category="EndlessRunner | Tiles")
	int32 NumOfMaxFloorTiles = 8;
	
	// Time until tile is completely destroyed, starts after character passes tile end.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EndlessRunner | Tiles")
	float TileLingerTime = 4.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="EndlessRunner | Tiles")
	TSubclassOf<ASKTileBase> DefaultTileClass;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="EndlessRunner | Debug")
	FTransform NextTileSpawnPoint;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="EndlessRunner | Debug")
	TArray<float> LaneLocationsOffset;

	UFUNCTION()
	void CreateInitialFloorTiles();
	UFUNCTION()
	ASKTileBase* AddFloorTile();

	protected:
	virtual void BeginPlay() override;

	private:
	void GetLaneCoordinates();
};
