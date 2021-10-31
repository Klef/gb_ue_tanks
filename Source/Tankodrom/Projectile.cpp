// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Tankodrom.h"
#include "Damageble.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

	//USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = SceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnMeshHit);
	RootComponent = Mesh;
	FireVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Effect"));
	FireVisualEffect->SetupAttachment(RootComponent);

// 	DestroyVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Destroy Effect"));
// 	DestroyVisualEffect->SetupAttachment(RootComponent);
// 
// 	DestroySoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Destroy Effect"));
// 	DestroySoundEffect->SetupAttachment(RootComponent);

}


void AProjectile::Start()
{
	StarPosition = GetActorLocation();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NextPosition, true);
	if (FVector::Dist(GetActorLocation(), StarPosition) > FireRange)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("puh"));
		Destroy();
	}
}

void AProjectile::OnMeshHit(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	//UE_LOG(LogTank, Verbose, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitVisualEffect, GetActorTransform().GetLocation(), GetActorTransform().GetRotation().Rotator(), FVector(3.0, 3.0, 3.0), true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSoundEffect, GetActorLocation());
	if (OtherActor == GetInstigator())
	{
		Destroy();
		return;
	}
	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else if (IDamageble * Damageable = Cast<IDamageble>(OtherActor))
	{
		FDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.Instigator = GetInstigator();
		DamageData.DamageMaker = this;
		Damageable->TakeDamage(DamageData);
	}
	Destroy();
}

