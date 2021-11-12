// Fill out your copyright notice in the Description page of Project Settings.


#include "SKObstacle.h"

#include "SKCharacter.h"

// Sets default values
ASKObstacle::ASKObstacle()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SceneComponent);
	StaticMesh->OnComponentHit.AddDynamic(this, &ASKObstacle::OnObstacleHit);
}

void ASKObstacle::OnObstacleHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ASKCharacter* PlayerCharacter = Cast<ASKCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->HandleHitDanger();
	}
}

// Called when the game starts or when spawned
void ASKObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}



