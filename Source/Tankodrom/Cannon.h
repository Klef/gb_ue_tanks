#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"


UCLASS()
class TANKODROM_API ACannon : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent * Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent * ProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent * ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent * AudioEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<class UCameraShakeBase> ShootShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin =1), Category = "Fire params")
	int32 FireSerialAmp = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "FireSerialAmp > 1", EditConditionHides), Category = "Fire params")
	float FireSerialRate = 3.0f; //for all

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditComdition = "Type == ECannonType::FireTrace;", EditConditionHides), Category = "Fire params")
	float FireRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int32 Ammo = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float ChargeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditComdition = "Type == ECannonType::FireProjectile;", EditConditionHides), Category = "Fire params")
	TSubclassOf<class AProjectile> ProjectileClass;

	

private:
	FTimerHandle ReloadTimerHandle;
	FTimerHandle ShotTimerHandle;
	FTimerHandle ChargeTimerHandle;
	bool bIsReadyToFire = false;
	bool bIsReCharge = false;
	int32 AmmoCurrent;
	int32 FireSerialCount = FireSerialAmp;
public:
	ACannon();
	void Shot();
	void Fire();
	void FireSpecial();
	bool IsReadyToFire();
	void ReAmmo();
	void ReCharge();
	void SetVisibility(bool bIsVisibility);
	void AddAmmo(int32 CountAmmo);
	bool NullAmmo();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	void Reload();

};
