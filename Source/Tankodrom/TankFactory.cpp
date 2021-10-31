// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "Tankodrom.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	DestroyBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Destroy Mesh"));
	DestroyBuildingMesh->SetupAttachment(SceneComp);

	SmokeDestroyVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Destroy Effect"));
	SmokeDestroyVisualEffect->SetupAttachment(DestroyBuildingMesh);

	SmokeDestroySoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Smoke Destroy Effect"));
	SmokeDestroySoundEffect->SetupAttachment(DestroyBuildingMesh);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->SetupAttachment(SceneComp);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(SceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddDynamic(this, &ATankFactory::Die);
	HealthComponent->OnHeathChange.AddDynamic(this, &ATankFactory::DamageTaked);

	SmokeVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Effect"));
	SmokeVisualEffect->SetupAttachment(BuildingMesh);

	FireVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Effect"));
	FireVisualEffect->SetupAttachment(BuildingMesh);

	SparksVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Sparks Effect"));
	SparksVisualEffect->SetupAttachment(BuildingMesh);

	SmokeSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Smoke Effect"));
	SmokeSoundEffect->SetupAttachment(BuildingMesh);

	FireSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Fire Effect"));
	FireSoundEffect->SetupAttachment(BuildingMesh);

	SparksSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Sparks Effect"));
	SparksSoundEffect->SetupAttachment(BuildingMesh);

	ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated lights"));
	ActivatedLight->SetupAttachment(BuildingMesh);
}

void ATankFactory::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTankTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
	DestroyBuildingMesh->SetHiddenInGame(true);
}

void ATankFactory::EndPlay(EEndPlayReason::Type EEndPlayReason)
{
	Super::EndPlay(EEndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTankTimerHandle);
}

void ATankFactory::SpawnNewTank()
{
	FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1.0f));
	ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//
	NewTank->SetPatrolPoints(TankWayPoints);
	//
	NewTank->FinishSpawning(SpawnTransform);
}


void ATankFactory::Die()
{
	if (MapLoader)
	{
		MapLoader->SetIsActivated(true);
	}
	ActivatedLight->SetHiddenInGame(true);
	BuildingMesh->SetHiddenInGame(true);
	DestroyBuildingMesh->SetHiddenInGame(false);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyVisualEffect, GetActorTransform().GetLocation(), GetActorTransform().GetRotation().Rotator(), FVector(3.0, 3.0, 3.0), true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySoundEffect, GetActorLocation());
	SmokeDestroySoundEffect->Play();
	SmokeDestroyVisualEffect->ActivateSystem();
	GetWorld()->GetTimerManager().ClearTimer(SpawnTankTimerHandle);
	SparksSoundEffect->Stop();
	SparksVisualEffect->DeactivateSystem();
	SmokeSoundEffect->Stop();
	SmokeVisualEffect->DeactivateSystem();
	FireVisualEffect->DeactivateSystem();
	FireSoundEffect->Stop();
	//Destroy();
}


void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTank, Verbose, TEXT("Factory %s taked damage: %f Heatlth %f. "), *GetName(), DamageValue, HealthComponent->GetHealht());
	if (!BIsFiring && HealthComponent->GetHealhtState() < 0.4f)
	{
		BIsFiring = true;
		FireVisualEffect->ActivateSystem();
		FireSoundEffect->Play();
	}
	if (!BIsSmoking && HealthComponent->GetHealhtState() < 0.8f)
	{
		BIsSmoking = true;
		SmokeSoundEffect->Play();
		SmokeVisualEffect->ActivateSystem();
	}
	if (!BIsSparks && HealthComponent->GetHealhtState() < 0.1f)
	{
		BIsSparks = true;
		SparksSoundEffect->Play();
		SparksVisualEffect->ActivateSystem();
	}
}


