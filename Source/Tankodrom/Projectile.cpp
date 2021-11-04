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
#include "PhisicMoventComponent.h"
#include "Kismet/KismetMathLibrary.h"

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


	MovementComponent = CreateDefaultSubobject<UPhisicMoventComponent>(TEXT("Moved Component"));

// 	DestroyVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Destroy Effect"));
// 	DestroyVisualEffect->SetupAttachment(RootComponent);
// 
// 	DestroySoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Destroy Effect"));
// 	DestroySoundEffect->SetupAttachment(RootComponent);

}


void AProjectile::Start()
{
	StarPosition = GetActorLocation();
	if (bIsPhisics)
	{
		MovementComponent->Velocity = GetActorForwardVector() * MoveSpeed;
		MovementComponent->SetComponentTickEnabled(true);
	}
	if (bIsRocket)
	{
		Mesh->BodyInstance.SetInstanceSimulatePhysics(true);
		Mesh->BodyInstance.bLockZTranslation = true;
		Mesh->SetEnableGravity(false);
	}
}

void AProjectile::Stop()
{
	if (bIsPhisics)
	{
		MovementComponent->Velocity = FVector::ZeroVector;
		MovementComponent->SetComponentTickEnabled(false);
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitVisualEffect, GetActorTransform().GetLocation(), GetActorTransform().GetRotation().Rotator(), FVector(3.0, 3.0, 3.0), true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSoundEffect, GetActorLocation());
	Destroy();
}

void AProjectile::Explosion(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	//Super::OnMeshHit(HittedComp, OtherActor, OtherComp, NormalImpulse, HitResult);
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplosionRange);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;

	FQuat Rotation = FQuat::Identity;
	GetWorld()->DebugDrawTraceTag = "Explode Trace";
	bool bSweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackHit,
		StartPos,
		EndPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		params
	);



	if (bSweepResult)
	{
		for (FHitResult HitResult : AttackHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (!HitActor)
			{
				continue;
			}

			IDamageble* DamageActor = Cast<IDamageble>(OtherActor);
			if (DamageActor)
			{
				FDamageData DamageData;
				DamageData.DamageValue = Damage;
				DamageData.Instigator = GetOwner();
				DamageData.DamageMaker = this;

				DamageActor->TakeDamage(DamageData);
			}

			UPrimitiveComponent* HitMesh = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
			if (HitMesh)
			{
				if (HitMesh->IsSimulatingPhysics())
				{
					FVector ForceVector = HitActor->GetActorLocation() - GetActorLocation();
					ForceVector.Normalize();
					HitMesh->AddImpulse(ForceVector * ExplosionImpule, NAME_None, true);
				}
			}
		}
	}

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsPhisics && !bIsRocket)
	{
		FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
		SetActorLocation(NextPosition, true);
	}
	if (bIsRocket)
	{
		float EngineRocketTrust = MoveSpeed;
		if (bIsRocketTarget)
		{
			
			EngineRocketTrust = MoveSpeed * InTargetSpeed;
			FVector CurentLocation = GetActorLocation();
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurentLocation, Target);
			FRotator CurrentRotation = Mesh->GetComponentRotation();
			Mesh->SetWorldRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, AgilitySmooth));


		}
		else
		{
			APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
			if (FVector::DistSquared(StarPosition, GetActorLocation()) > FMath::Square(EnableRange))
			{
				TArray<AActor*> FindTanks;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FindTanks);
				float Min = FMath::Square(FireRange);
				for (AActor * Actor : FindTanks)
				{
					APawn* PotencialTarget = Cast<APawn>(Actor);
					if (PotencialTarget == PlayerPawn)
					{
						continue;
					}
					if (FVector::DistSquared(PotencialTarget->GetActorLocation(), GetActorLocation()) < Min)
					{
						Target = PotencialTarget->GetActorLocation();
						
						Min = FVector::DistSquared(PotencialTarget->GetActorLocation(), GetActorLocation());
					}
				}
				if (Target == FVector::ZeroVector)
				{
					Stop();
				}
				else
				{
					bIsRocketTarget = true;
				}
			}
		}

		Mesh->AddForce(GetActorForwardVector() * EngineRocketTrust);
	}
	if (bIsPhisics)
	{
		if (GetActorLocation().Z < -10000.0f)
		{
			Stop();
		}
	}
	else
	{
		if (FVector::Dist(GetActorLocation(), StarPosition) > FireRange)
		{
			Stop();
		}
	}
}

float AProjectile::GetMoveSpeed()
{
	return MoveSpeed;
}

UPhisicMoventComponent * AProjectile::GetMoveClass()
{
	return MovementComponent;
}

void AProjectile::OnMeshHit(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	UE_LOG(LogTank, Verbose, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	if (bIsExplosion)
	{
		Explosion(HittedComp, OtherActor, OtherComp, NormalImpulse, HitResult);
	}
	else
	{
		if (OtherComp->IsSimulatingPhysics())
		{
			FVector Impulse = Mass * MoveSpeed * GetActorForwardVector();
			OtherComp->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint);

		}
		else if (IDamageble* Damageable = Cast<IDamageble>(OtherActor))
		{
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.Instigator = GetInstigator();
			DamageData.DamageMaker = this;
			Damageable->TakeDamage(DamageData);
		}
	}
	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	
	Stop();
}

