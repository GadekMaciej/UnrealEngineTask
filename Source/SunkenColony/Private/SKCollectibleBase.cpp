// Fill out your copyright notice in the Description page of Project Settings.


#include "SKCollectibleBase.h"

// Sets default values
ASKCollectibleBase::ASKCollectibleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASKCollectibleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASKCollectibleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

