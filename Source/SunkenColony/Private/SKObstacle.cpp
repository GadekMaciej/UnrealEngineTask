// Fill out your copyright notice in the Description page of Project Settings.


#include "SKObstacle.h"

// Sets default values
ASKObstacle::ASKObstacle()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SceneComponent);

	
}

// Called when the game starts or when spawned
void ASKObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}



