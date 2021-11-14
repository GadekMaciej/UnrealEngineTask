// Fill out your copyright notice in the Description page of Project Settings.


#include "SKCollectibleBase.h"

#include "SKCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASKCollectibleBase::ASKCollectibleBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");
	RootComponent = SceneComponent;

	SphereCollider = CreateDefaultSubobject<USphereComponent>("Sphere Collider");
	SphereCollider->SetupAttachment(SceneComponent);
	SphereCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(SphereCollider);
	StaticMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("Rotating Movement Component");
	RotatingMovementComponent->RotationRate = FRotator(0, 90, 0);
}

void ASKCollectibleBase::OnSphereColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASKCharacter* Character = Cast<ASKCharacter>(OtherActor);
	if(Character)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnCollectSound, GetActorLocation());
		if(GetActorLocation().Z + HeightDifferenceRequiredForJumpedOn < OtherActor->GetActorLocation().Z)
		{
			OnJumpedOnTop(Character);
			if(bOnJumpedOnTopCallsOnCollected)
			{
				OnCollected(Character);
			}
		}
		else
		{
			OnCollected(Character);
		}
		Destroy();
	}
}

// Called when the game starts or when spawned
void ASKCollectibleBase::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ASKCollectibleBase::OnSphereColliderOverlap);
}


