// Fill out your copyright notice in the Description page of Project Settings.


#include "SKTileBase.h"

#include "SKCharacter.h"
#include "SKEndlessRunnerGM.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASKTileBase::ASKTileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root Component"));
	RootComponent = SceneComponent;
	
	FloorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Static Mesh"));
	FloorStaticMesh->SetupAttachment(SceneComponent);
	
	NextTileAttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Next Tile Attach Point"));
	NextTileAttachPoint->SetupAttachment(SceneComponent);
	PreviousTileAttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Previous Tile Attach Point"));
	PreviousTileAttachPoint->SetupAttachment(SceneComponent);

	LaneA = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Lane"));
	LaneA->SetupAttachment(SceneComponent);
	LaneB = CreateDefaultSubobject<UArrowComponent>(TEXT("Middle Lane"));
	LaneB->SetupAttachment(SceneComponent);
	LaneC = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Lane"));
	LaneC->SetupAttachment(SceneComponent);

	CollisionBoxBegin = CreateDefaultSubobject<UBoxComponent>("Collision box begin");
	CollisionBoxBegin->SetupAttachment(SceneComponent);
	CollisionBoxBegin->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	CollisionBoxEnd = CreateDefaultSubobject<UBoxComponent>("Collision box end");
	CollisionBoxEnd->SetupAttachment(SceneComponent);
	CollisionBoxEnd->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void ASKTileBase::OnEnteringTile_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ASKTileBase::OnExitingTile_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASKCharacter* Character = Cast<ASKCharacter>(OtherActor);
	if (Character)
	{
		ASKEndlessRunnerGM* GM = Cast<ASKEndlessRunnerGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->AddFloorTile();
			GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASKTileBase::HandleDestroyFloorTile, GM->TileLingerTime);
		}
	}
}

void ASKTileBase::HandleDestroyFloorTile()
{
	if(DestroyTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	}
	this->Destroy();
}

// Called when the game starts or when spawned
void ASKTileBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionBoxBegin->OnComponentBeginOverlap.AddDynamic(this, &ASKTileBase::OnEnteringTile);
	CollisionBoxEnd->OnComponentBeginOverlap.AddDynamic(this, &ASKTileBase::OnExitingTile);
}

// Called every frame
void ASKTileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

