// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"


// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	bIsReadyToFire = true;
}


void ACannon::SerialShot()
{
	if (FireSerialCount == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);
		FireSerialCount = FireSerialAmp;
	}
	else
	{
		FireSerialCount--;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Orange, FString::Printf(TEXT("fire # %d"), FireSerialAmp - FireSerialCount));
	}
}

void ACannon::Fire()
{
	if (bIsReadyToFire)
	{
		if (AmmoCurrent == 0)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, TEXT("NO AMMO"));
			bIsReadyToFire = false;
		}
		if (Type == ECannonType::FireProjectile)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("Fire Projectile"));
		}
		if (Type == ECannonType::FireTrace)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("Fire Trace"));
		}

		AmmoCurrent--;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, FString::Printf(TEXT("AMMO: %d"), AmmoCurrent));
		GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ACannon::SerialShot, 1.0f, false);
		GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ACannon::SerialShot, 1.0f / FireRate, true, 1.0f / FireSerialRate);
		bIsReadyToFire = false;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.0f / FireRate , false, 1.0f / FireRate + FireSerialAmp * 1.0f / FireSerialRate);
	}
	if (bIsReCharge)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, TEXT("Waiting Charge"));
	}
}



void ACannon::FireSpecial()
{

	if (bIsReadyToFire)
	{
		if (AmmoCurrent == 0)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, TEXT("NO AMMO"));
			bIsReadyToFire = false;
		}
		if (Type == ECannonType::FireProjectile)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("Fire Projectile"));
		}
		if (Type == ECannonType::FireTrace)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("Fire Trace"));
		}
		//допустим это картечь
		AmmoCurrent--;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, FString::Printf(TEXT("AMMO: %d"), AmmoCurrent));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Orange, TEXT("BOOM"));
		bIsReadyToFire = false;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.0f / FireRate, false);
	}
	if (bIsReCharge)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, TEXT("Waiting Charge"));
	}

}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	
	GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}

void ACannon::ReAmmo()
{
	AmmoCurrent = Ammo;
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, TEXT("FULL AMMO"));
	bIsReadyToFire = true;
	bIsReCharge = false;
}

void ACannon::ReCharge()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, TEXT("RELOADING"));
	bIsReadyToFire = false;
	bIsReCharge = true;
	GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &ACannon::ReAmmo, ChargeTime, false);
}
