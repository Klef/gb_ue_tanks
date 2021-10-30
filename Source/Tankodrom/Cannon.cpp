// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "Damageble.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"


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

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
	AudioEffect->SetupAttachment(ProjectileSpawnPoint);

}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	bIsReadyToFire = true;
	AmmoCurrent = Ammo;
}


void ACannon::Shot()
{
	float Tic = FireSerialRate / FireSerialAmp;
	if (FireSerialAmp == 1)
	{
		Tic = 0.01f;
	}
	if (FireSerialCount > 0)
	{
		ShootEffect->ActivateSystem();
		AudioEffect->Play();
		if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if (ShootShake)
			{
				GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootShake);
			}
		}
		--FireSerialCount;
		if (Type == ECannonType::FireProjectile)
		{
			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("Fire Projectile"));
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			if (Projectile)
			{
				Projectile->SetInstigator(GetInstigator());
				Projectile->Start();
			}
		}
		if (Type == ECannonType::FireTrace)
		{
			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("Fire Trace"));
			FHitResult HitResult;
			FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
			FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
			FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
			TraceParams.bReturnPhysicalMaterial = false;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
			{
				DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 10.0f);
				if (HitResult.Actor.IsValid() && HitResult.Component.IsValid(), HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
				{
					HitResult.Actor->Destroy();
				}
				else if (IDamageble* Damageable = Cast<IDamageble>(HitResult.Actor))
				{
					if (HitResult.Actor != GetInstigator())
					{
						FDamageData DamageData;
						DamageData.DamageValue = FireDamage;
						DamageData.Instigator = GetInstigator();
						DamageData.DamageMaker = this;
						Damageable->TakeDamage(DamageData);
					}
				}
			}
			else
			{
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.0f);
			}
		}
		GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ACannon::Shot, FireSerialRate / FireSerialAmp, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.0f / FireRate, false);
	}
}

void ACannon::Fire()
{
	if (bIsReadyToFire)
	{
		if (AmmoCurrent != 0)
		{
			AmmoCurrent--;
			bIsReadyToFire = false;
			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, FString::Printf(TEXT("AMMO: %d"), AmmoCurrent));
			GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ACannon::Shot, 0.01f, false);
		}
		
	}
// 	if (bIsReCharge)
// 	{
// 		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, TEXT("Waiting Charge"));
// 	}
}



void ACannon::FireSpecial()
{

	if (bIsReadyToFire)
	{
		if (AmmoCurrent != 0)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Orange, TEXT("BOOM"));
		}
	}
// 	if (bIsReCharge)
// 	{
// 		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, TEXT("Waiting Charge"));
// 	}

}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

void ACannon::Reload()
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, TEXT("Ready"));
	bIsReadyToFire = true;
	FireSerialCount = FireSerialAmp;
}

void ACannon::ReAmmo()
{
	AmmoCurrent = Ammo;
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, TEXT("FULL AMMO"));
	bIsReadyToFire = true;
	bIsReCharge = false;
}

void ACannon::ReCharge()
{
	if (!bIsReCharge)
	{
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, TEXT("RELOADING"));
		bIsReadyToFire = false;
		bIsReCharge = true;
		GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &ACannon::ReAmmo, ChargeTime, false);
	}
}

void ACannon::SetVisibility(bool bIsVisibility)
{
	Mesh->SetHiddenInGame(!bIsVisibility);
}

void ACannon::AddAmmo(int32 CountAmmo)
{
	AmmoCurrent += CountAmmo;
}

bool ACannon::NullAmmo()
{
	return AmmoCurrent == 0;
}
