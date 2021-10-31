// Fill out your copyright notice in the Description page of Project Settings.


#include "Turet.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "Tankodrom.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATuret::ATuret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;
	//38.11

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (TurretMeshTemp)
	{
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}
	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}

	HitVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Hit Effect"));
	HitVisualEffect->SetupAttachment(TurretMesh);



	SmokeVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Effect"));
	SmokeVisualEffect->SetupAttachment(TurretMesh);

	FireVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Effect"));
	FireVisualEffect->SetupAttachment(TurretMesh);

	SparksVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Sparks Effect"));
	SparksVisualEffect->SetupAttachment(TurretMesh);


	HitSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio HIt Effect"));
	HitSoundEffect->SetupAttachment(TurretMesh);


	SmokeSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Smoke Effect"));
	SmokeSoundEffect->SetupAttachment(TurretMesh);

	FireSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Fire Effect"));
	FireSoundEffect->SetupAttachment(TurretMesh);

	SparksSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Sparks Effect"));
	SparksSoundEffect->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHeathChange.AddDynamic(this, &ATuret::OnHeathChange);
	HealthComponent->OnDie.AddDynamic(this, &ATuret::OnDie);
}

// Called when the game starts or when spawned
void ATuret::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

void ATuret::Destroyed()
{
	Super::Destroyed();
	if (Cannon)
	{
		Cannon->Destroy();
	}
}

void ATuret::DestroyWait()
{
	Destroy();
}

void ATuret::Targeting()
{
 	if (IsPlayerInRange())
 	{
 		RotateToPlayer();
 	}
 
 	if (CanFire() && Cannon && Cannon->IsReadyToFire())
 	{
 		Fire();
 	}
}

void ATuret::RotateToPlayer()
{

	FHitResult HitResult;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = PlayerPawn->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Turet Vision Trace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
	{
		
		if (HitResult.Actor != PlayerPawn)
		{
			return;
		}
	}

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrRotation.Pitch;
	TargetRotation.Roll = CurrRotation.Roll;

	
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}

bool ATuret::IsPlayerInRange()
{
	return FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) <= FMath::Square(TargetingRange);
}

bool ATuret::CanFire()
{
	FVector TargetingDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
	return AimAngle <= Accuracy;
}

void ATuret::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
		if (Cannon->NullAmmo())
		{
			Cannon->ReCharge();
		}
	}
}

void ATuret::OnHeathChange_Implementation(float Damage)
{
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

void ATuret::OnDie_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyVisualEffect, GetActorTransform().GetLocation(), GetActorTransform().GetRotation().Rotator(), FVector(3.0, 3.0, 3.0), true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySoundEffect, GetActorLocation());
	Destroy();
	//GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ATuret::DestroyWait, 0.5f, false);
}

void ATuret::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
}

// Called every frame
void ATuret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerPawn)
	{
		Targeting();
	}
}

void ATuret::TakeDamage(const FDamageData& DamageData)
{
	
	HealthComponent->TakeDamage(DamageData);
}

