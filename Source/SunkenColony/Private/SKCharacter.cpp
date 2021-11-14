// Fill out your copyright notice in the Description page of Project Settings.


#include "SKCharacter.h"

#include "SKEndlessRunnerGM.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "SKAbilitySystemComponent.h"
#include "SKAttributeSet.h"
#include "SKData.h"
#include "SKGameplayAbility.h"

ASKCharacter::ASKCharacter()
{
	bAbilitiesInitialized = false;
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystemComponent = CreateDefaultSubobject<USKAbilitySystemComponent>("Ability System Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<USKAttributeSet>("Attributes");

	// GAS Attributes OnChanged Binds
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddUObject(this, &ASKCharacter::OnHealthAttributeModified);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMoveSpeedAttribute()).AddUObject(this, &ASKCharacter::OnMoveSpeedAttributeModified);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetLaneSwitchSpeedAttribute()).AddUObject(this, &ASKCharacter::OnLaneSwitchSpeedAttributeModified);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ASKCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!bIsDead)
	{
		FRotator ControlRotator = GetControlRotation();
		ControlRotator.Roll = 0.0f;
		ControlRotator.Pitch = 0.0f;
		AddMovementInput(ControlRotator.Vector());
	}
}

void ASKCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//server GAS init (server only)
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGamePlayAbilities();
	}
}

void ASKCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"SKAbilityInputID",
			static_cast<int32>(SKAbilityInputID::Confirm),
			static_cast<int32>(SKAbilityInputID::Confirm));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void ASKCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"SKAbilityInputID",
			static_cast<int32>(SKAbilityInputID::Confirm),
			static_cast<int32>(SKAbilityInputID::Confirm));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
	
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ASKCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ASKCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASKCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASKCharacter::StopJumping);
	
}

void ASKCharacter::ChangeLane_Implementation()
{
	if (LaneSwitchParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaneSwitchParticleSystem, GetActorLocation());
	}
	if (LaneSwitchSoundEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaneSwitchSoundEffect, GetActorLocation());
	}
}

void ASKCharacter::ChangeLaneUpdate(const float Value)
{
	FVector CharacterLocation = GetCapsuleComponent()->GetComponentLocation();
	ASKEndlessRunnerGM* GM = Cast<ASKEndlessRunnerGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		CharacterLocation.Y = FMath::Lerp(GM->LaneLocationsOffset[CurrentLane], GM->LaneLocationsOffset[NextLane], Value);
		SetActorLocation(CharacterLocation);
	}
}

void ASKCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ASKCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();
}

void ASKCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();
}

void ASKCharacter::HandleDeath()
{
	if (!bIsDead)
	{
		bIsDead = true;
		GetMesh()->SetSimulatePhysics(true);
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		const FVector Location = GetActorLocation();
		UWorld* World = GetWorld();
		if (World)
		{
			if(DeathParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticleSystem, Location);
			}
			if(DeathSoundEffect)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSoundEffect, Location);
			}

			// call restart game
			ASKEndlessRunnerGM* GM = Cast<ASKEndlessRunnerGM>(UGameplayStatics::GetGameMode(World));
			World->GetTimerManager().SetTimer(GM->LevelRestartTimerHandle, GM, &ASKEndlessRunnerGM::HandleGameOver, GM->GameOverLingerTimer);
		} 
	}
}

void ASKCharacter::HandleHitDanger()
{
	HandleDeath();
}

float ASKCharacter::GetHealthAttribute()
{
	if (Attributes)
	{
		return Attributes->GetHealth();
	}
	return -1.f;
}

float ASKCharacter::GetMaxHealthAttribute()
{
	if (Attributes)
	{
		return Attributes->GetMaxHealth();
	}
	return -1.f;

}

float ASKCharacter::GetMoveSpeedAttribute()
{
	if (Attributes)
	{
		return Attributes->GetMoveSpeed();
	}
	return -1.f;

}

float ASKCharacter::GetMaxMoveSpeedAttribute()
{
	if (Attributes)
	{
		return Attributes->GetMaxMoveSpeed();
	}
	return -1.f;

}

float ASKCharacter::GetLaneSwitchSpeedAttribute()
{
	if (Attributes)
	{
		return Attributes->GetLaneSwitchSpeed();
	}
	return -1.f;

}

float ASKCharacter::GetMaxLaneSwitchSpeedAttribute()
{
	if (Attributes)
	{
		return Attributes->GetMaxLaneSwitchSpeed();
	}
	return -1.f;

}

void ASKCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void ASKCharacter::HandleMoveSpeedChanged(float DeltaValue, float OverrideValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		if(!FMath::IsNearlyEqual(OverrideValue, -1.f))
		{
			GetCharacterMovement()->MaxWalkSpeed = OverrideValue;
		}
		if(!FMath::IsNearlyEqual(DeltaValue, -1.f))
		{
			GetCharacterMovement()->MaxWalkSpeed += DeltaValue;
		}
		
		OnMoveSpeedChanged(DeltaValue, EventTags);
	}
}

void ASKCharacter::HandleLaneSwitchSpeedChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnLaneSwitchSpeedChanged(DeltaValue, EventTags);
	}
}

void ASKCharacter::OnHealthAttributeModified(const FOnAttributeChangeData& Data)
{
	// TODO Health is not utilized as resource yet
}

void ASKCharacter::OnMoveSpeedAttributeModified(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void ASKCharacter::OnLaneSwitchSpeedAttributeModified(const FOnAttributeChangeData& Data)
{
	SwitchLaneTimeLineCPP->SetPlayRate(Data.NewValue);
}

void ASKCharacter::AddStartupGamePlayAbilities()
{
	check(AbilitySystemComponent)
	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		bAbilitiesInitialized = true;
		// Grant active abilities (only on the server)
		for (TSubclassOf<USKGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this)
				);
		}

		// Apply passive effects
		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);

			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
					*NewHandle.Data.Get(),
					AbilitySystemComponent
					);
			}
		}
	}
}

UAbilitySystemComponent* ASKCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASKCharacter::Jump()
{
	if (!bIsDead)
	{
		Super::Jump();
	}
}

void ASKCharacter::StopJumping()
{
	if (!bIsDead)
	{
		Super::StopJumping();
	}
}



