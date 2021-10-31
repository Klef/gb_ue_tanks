// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "Tankodrom.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->SetupAttachment(SceneComp);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(SceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddDynamic(this, &ATankFactory::Die);
	HealthComponent->OnHeathChange.AddDynamic(this, &ATankFactory::DamageTaked);
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
	Destroy();
}


void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTank, Verbose, TEXT("Factory %s taked damage: %f Heatlth %f. "), *GetName(), DamageValue, HealthComponent->GetHealht());
}

