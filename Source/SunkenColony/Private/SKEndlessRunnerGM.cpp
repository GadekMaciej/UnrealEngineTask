// Fill out your copyright notice in the Description page of Project Settings.


#include "SKEndlessRunnerGM.h"
#include "SKTileBase.h"
#include "SunkenColony/SunkenColony.h"

ASKEndlessRunnerGM::ASKEndlessRunnerGM()
{
	
}

void ASKEndlessRunnerGM::CreateInitialFloorTiles()
{
	// this function creates 1 additional tile, just something to fix later
	GetLaneCoordinates();
	for (int i = 0; i < NumOfInitFloorTiles-1; i++)
	{
		AddFloorTile();
	}
	UE_LOG(SunkenColony, Log, TEXT("Created Initial Tiles"));
}

ASKTileBase* ASKEndlessRunnerGM::AddFloorTile()
{
	ASKTileBase* NewTile = nullptr;
	UWorld* World = GetWorld();
	if (World)
	{
		NewTile = World->SpawnActor<ASKTileBase>(DefaultTileClass, NextTileSpawnPoint);
		if (NewTile)
		{
			NextTileSpawnPoint = NewTile->GetNextTileAttachTransform();
		}
	}
	return NewTile;
}

void ASKEndlessRunnerGM::BeginPlay()
{
	Super::BeginPlay();
	CreateInitialFloorTiles();
}

void ASKEndlessRunnerGM::GetLaneCoordinates()
{
	ASKTileBase* NewTile = AddFloorTile();
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
