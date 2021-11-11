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

	// Initial floor tiles are blank. There are no obstacles nor powerups.
	UPROPERTY(EditDefaultsOnly, Category="EndlessRunner | Tiles")
	int32 NumOfInitFloorTiles = 8;
	
	// Time until tile is completely destroyed, starts after character passes tile end.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EndlessRunner | Tiles")
	float TileLingerTime = 4.0f;

	// Tile class that will be used as initial tiles.
	UPROPERTY(EditDefaultsOnly, Category="EndlessRunner | Tiles")
	TSubclassOf<ASKTileBase> DefaultTileClass;
	
	// An Array of tile classes that will be generated in endless loop. For now they all have the same priority.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EndlessRunner | Tiles")
	TArray<TSubclassOf<ASKTileBase>> TypesOfTilesGenerated;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="EndlessRunner | Debug")
	FTransform NextTileSpawnPoint;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="EndlessRunner | Debug")
	TArray<float> LaneLocationsOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EndlessRunner | Game")
	// Time That the game will wait after player lost all lifes.
	float GameOverLingerTimer = 4.0f;

	UPROPERTY()
	FTimerHandle LevelRestartTimerHandle;
	
	UFUNCTION(BlueprintNativeEvent, Category="Tiles")
	void CreateInitialFloorTiles();
	
	UFUNCTION(BlueprintCallable, Category="Tiles")
	ASKTileBase* AddFloorTile(const bool bUseDefaultFloorTile = false);

	UFUNCTION(BlueprintNativeEvent, Category="Game")
	void HandleGameOver();

	protected:
	virtual void BeginPlay() override;

	private:
	void GetLaneCoordinates();
	TSubclassOf<ASKTileBase> RandomizeTileType();
};
