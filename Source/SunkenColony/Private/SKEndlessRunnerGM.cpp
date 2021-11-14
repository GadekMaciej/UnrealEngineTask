// Fill out your copyright notice in the Description page of Project Settings.


#include "SKEndlessRunnerGM.h"
#include "SKTileBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SunkenColony/SunkenColony.h"

ASKEndlessRunnerGM::ASKEndlessRunnerGM()
{
	
}

void ASKEndlessRunnerGM::BeginPlay()
{
	Super::BeginPlay();
	CreateInitialFloorTiles();
}

void ASKEndlessRunnerGM::CreateInitialFloorTiles_Implementation()
{
	// this function creates 1 additional tile, just something to fix later
	GetLaneCoordinates();
	for (int i = 0; i < NumOfInitFloorTiles-1; i++)
	{
		AddFloorTile(true);
	}
	UE_LOG(SunkenColony, Log, TEXT("Created Initial Tiles"));
}

void ASKEndlessRunnerGM::HandleGameOver_Implementation()
{
	if (LevelRestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(LevelRestartTimerHandle);
	}
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("RestartLevel"));
}

ASKTileBase* ASKEndlessRunnerGM::AddFloorTile(const bool bUseDefaultFloorTile)
{
	ASKTileBase* NewTile = nullptr;
	UWorld* World = GetWorld();
	if (World)
	{
		if (bUseDefaultFloorTile)
		{
			
			NewTile = World->SpawnActor<ASKTileBase>(DefaultTileClass);
			if (NewTile)
			{				
				FVector NewLocation = NextTileSpawnPoint.GetTranslation() + NewTile->PreviousTileAttachPoint->GetRelativeLocation().GetAbs();
				NewTile->SetActorLocation(NewLocation);
				NextTileSpawnPoint = NewTile->GetNextTileAttachTransform();
			}
		}
		else
		{
			NewTile = World->SpawnActor<ASKTileBase>(RandomizeTileType());
			if (NewTile)
			{
				FVector NewLocation = NextTileSpawnPoint.GetTranslation() + NewTile->PreviousTileAttachPoint->GetRelativeLocation().GetAbs();
				NewTile->SetActorLocation(NewLocation);
				NextTileSpawnPoint = NewTile->GetNextTileAttachTransform();
			}
		}
	}
	return NewTile;
}

void ASKEndlessRunnerGM::ModifyPlayerScore(int32 Value)
{
	PlayerScore += Value;
	PlayerScoreChanged.Broadcast(PlayerScore, Value);
}

void ASKEndlessRunnerGM::GetLaneCoordinates()
{
	ASKTileBase* NewTile = AddFloorTile(true);
	if (NewTile)
	{
		LaneLocationsOffset.Add(NewTile->LaneA->GetComponentLocation().Y);
		LaneLocationsOffset.Add(NewTile->LaneB->GetComponentLocation().Y);
		LaneLocationsOffset.Add(NewTile->LaneC->GetComponentLocation().Y);

		for (float LaneCoord : LaneLocationsOffset)
		{
			UE_LOG(SunkenColony, Log, TEXT("Lane Coordinate is: %f"), LaneCoord);
		}
	}
	else
	{
		UE_LOG(SunkenColony, Log, TEXT("Could not determine lanes coordinates, make sure class tile is selected inside GameMode"))
	}
}

TSubclassOf<ASKTileBase> ASKEndlessRunnerGM::RandomizeTileType()
{
	const int32 RandomIndex = FMath::RandRange(0, TypesOfTilesGenerated.Num()-1);
	return TypesOfTilesGenerated[RandomIndex];
}

