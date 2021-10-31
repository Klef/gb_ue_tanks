// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhisicMoventComponent.h"
#include "PhisicProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TANKODROM_API APhisicProjectile : public AProjectile
{
	GENERATED_BODY()
public:
	APhisicProjectile();
	virtual void Start() override;
	virtual void Stop() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float ExplosionRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float ExplosionImpule = 100.0f; 

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPhisicMoventComponent* MovementComponent;

	virtual void OnMeshHit(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector  NormalImpulse, const FHitResult& HitResult) override;
};
