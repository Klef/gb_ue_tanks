// Fill out your copyright notice in the Description page of Project Settings.


#include "PhisicProjectile.h"
#include "Damageble.h"

APhisicProjectile::APhisicProjectile()
{
	MovementComponent = CreateDefaultSubobject<UPhisicMoventComponent>(TEXT("Moved Component"));
}

void APhisicProjectile::Start()
{
	Super::Start();
	MovementComponent->Velocity = GetActorForwardVector() * MoveSpeed;
	MovementComponent->SetComponentTickEnabled(true);

}

void APhisicProjectile::Stop()
{
	MovementComponent->Velocity = FVector::ZeroVector;
	MovementComponent->SetComponentTickEnabled(false);

	Super::Stop();
}

void APhisicProjectile::Tick(float DeltaSeconds)
{
	if (GetActorLocation().Z < -10000.0f)
	{
		Stop();
	}
}

void APhisicProjectile::OnMeshHit(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
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
			if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
			{
				OtherActor->Destroy();
			}
		}
	}
	Stop();
}
