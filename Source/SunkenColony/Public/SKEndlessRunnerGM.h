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

protected:
	virtual void BeginPlay() override;

	// ******************************************
	// ******** Editor Exposed Properties *******
	// ******************************************
private:
	// Initial floor tiles are blank. There are no obstacles nor powerups
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="EndlessRunner | Tiles")
	int32 NumOfInitFloorTiles = 8;
	
	// Time until tile is completely destroyed, starts after character passes tile end
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="EndlessRunner | Tiles")
	float TileLingerTime = 4.0f;

	// Tile class that will be used as initial tiles.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="EndlessRunner | Tiles")
	TSubclassOf<ASKTileBase> DefaultTileClass;
	
	// An Array of tile classes that will be generated in endless loop. For now they all have the same priority
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="EndlessRunner | Tiles")
	TArray<TSubclassOf<ASKTileBase>> TypesOfTilesGenerated;
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="EndlessRunner | Debug")
	FTransform NextTileSpawnPoint;

	// all 3 Lane X coordinates, Character only moves along X axis so its all we need
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="EndlessRunner | Debug")
	TArray<float> LaneLocationsOffset;
	
	// Time That the game will wait after player lost all lifes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="EndlessRunner | Game")
	float GameOverLingerTimer = 4.0f;

	// Player score, various pickups can increase it
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="EndlessRunner | Game")
	int32 PlayerScore = 0;
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~ Editor Exposed Properties ~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// ******************************************
	// ************ Blueprint Events ************
	// ******************************************
public:
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FPlayerScoreChanged PlayerScoreChanged;
	
	UFUNCTION(BlueprintNativeEvent, Category="Tiles")
	void CreateInitialFloorTiles();
	
	UFUNCTION(BlueprintNativeEvent, Category="Game")
	void HandleGameOver();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~ Blueprint Events ~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// *****************************************
	// ********** Blueprint Callables **********
	// *****************************************
	
	UFUNCTION(BlueprintCallable, Category="Tiles")
	ASKTileBase* AddFloorTile(const bool bUseDefaultFloorTile = false);

	UFUNCTION(BlueprintCallable, Category="Game")
	void ModifyPlayerScore(int32 Value);
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~ Blueprint Callables ~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	private:
	FTimerHandle LevelRestartTimerHandle;
	void GetLaneCoordinates();
	TSubclassOf<ASKTileBase> RandomizeTileType();
};
