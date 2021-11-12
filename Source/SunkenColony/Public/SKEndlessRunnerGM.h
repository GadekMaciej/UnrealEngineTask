// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SKEndlessRunnerGM.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerScoreChanged, int32, NewScore, int32, ScoreChange);

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EndlessRunner | Tiles")
	int32 NumOfInitFloorTiles = 8;
	
	// Time until tile is completely destroyed, starts after character passes tile end.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EndlessRunner | Tiles")
	float TileLingerTime = 4.0f;

	// Tile class that will be used as initial tiles.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EndlessRunner | Tiles")
	TSubclassOf<ASKTileBase> DefaultTileClass;
	
	// An Array of tile classes that will be generated in endless loop. For now they all have the same priority.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EndlessRunner | Tiles")
	TArray<TSubclassOf<ASKTileBase>> TypesOfTilesGenerated;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="EndlessRunner | Debug")
	FTransform NextTileSpawnPoint;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="EndlessRunner | Debug")
	TArray<float> LaneLocationsOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EndlessRunner | Game")
	// Time That the game will wait after player lost all lifes.
	float GameOverLingerTimer = 4.0f;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category="EndlessRunner | Game")
	int32 PlayerScore = 0;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FPlayerScoreChanged PlayerScoreChanged;
	
	FTimerHandle LevelRestartTimerHandle;
	
	UFUNCTION(BlueprintNativeEvent, Category="Tiles")
	void CreateInitialFloorTiles();
	
	UFUNCTION(BlueprintCallable, Category="Tiles")
	ASKTileBase* AddFloorTile(const bool bUseDefaultFloorTile = false);

	UFUNCTION(BlueprintNativeEvent, Category="Game")
	void HandleGameOver();

	UFUNCTION(BlueprintCallable, Category="Game")
	void ModifyPlayerScore(int32 Value);

	protected:
	virtual void BeginPlay() override;

	private:
	void GetLaneCoordinates();
	TSubclassOf<ASKTileBase> RandomizeTileType();
};
