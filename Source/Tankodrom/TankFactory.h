// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageble.h"
#include "HealthComponent.h"
#include "TankPawn.h"
#include "Engine/TargetPoint.h"
#include "MapLoader.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "TankFactory.generated.h"

UCLASS()
class TANKODROM_API ATankFactory : public AActor, public IDamageble
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankFactory();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BuildingMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* DestroyBuildingMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* TankSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UParticleSystem* DestroyVisualEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* SmokeVisualEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* SmokeDestroyVisualEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* FireVisualEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* SparksVisualEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPointLightComponent* ActivatedLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USoundBase* DestroySoundEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* SmokeSoundEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* SmokeDestroySoundEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* FireSoundEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* SparksSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<class ATankPawn> SpawnTankClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
	float SpawnTankRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TArray<class ATargetPoint*> TankWayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal params")
	class AMapLoader * MapLoader;
public:
	virtual void TakeDamage(const FDamageData& DamageData) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EEndPlayReason) override;

	void SpawnNewTank();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);

	bool bIsActive = true;
	bool BIsFiring = false;
	bool BIsSmoking = false;
	bool BIsSparks = false;

private:
	FTimerHandle SpawnTankTimerHandle;
};
