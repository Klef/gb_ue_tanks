// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Damageble.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"
#include "TankPawn.generated.h"


class UStaticMeshComponent;
class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TANKODROM_API ATankPawn : public APawn, public IDamageble
{
	GENERATED_BODY()

public:
	ATankPawn();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float AxisValue);


	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTurretTargetPosition(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMousePosition(FVector MouseValue);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void FireSpecial();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void ReCharge();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* SmokeSoundEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* FireSoundEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* SmokeVisualEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* FireVisualEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	FName DeathLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	bool bIsInput = true;

	UFUNCTION(BlueprintPure, Category = "Turret")
	FVector GetTurretForwardVector();

// 	UFUNCTION(BlueprintCallable, Category = "Turret")
// 	void ChangeCannon();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupCannon(TSubclassOf<class ACannon> InCannonClass);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupAlterCanon(TSubclassOf<class ACannon> InCannonClass);

	void AddAmmo(int32 CountAmmo);
	bool NullAmmo();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHeathChange(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();

	virtual void TakeDamage(const FDamageData& DamageData) override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "AI|Move params")
	void SetPatrolPoints(TArray<class ATargetPoint*>& InPoints)
	{
		PatrollingPoint = InPoints;
	}

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	const TArray<class ATargetPoint*>& GetPatrolPoints() const
	{
		return PatrollingPoint;
	};

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	float GetMovementAccuracy()
	{
		return MovementAccuracy;
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	FName LoadLevelName;

	FName GetLevelName()
	{
		return LoadLevelName;
	}
	bool bIsFiring = false;
	bool bIsSmoking = false;
	void HitEffect();
	void SetLoaderBool(bool bIsLoad)
	{
		bIsInput = bIsLoad;
	}
	void SetDeathName(FName LevelValue)
	{
		DeathLevel = LevelValue;
	}
	void Load();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
//	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
//	void DestroyWait();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent * ArmorMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent * TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* AlterTurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonAlterSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
 	class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitTurretCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

// 	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
// 	class UParticleSystemComponent* HitVisualEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UParticleSystem* DestroyVisualEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* SparksVisualEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* EngineVisualEffect;

// 	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
// 	class UAudioComponent* HitSoundEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USoundBase* DestroySoundEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* SparksSoundEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* EngineSoundEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPointLightComponent* LightReadyCannon;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPointLightComponent* LightReadyAltCannon;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPointLightComponent* LightBusyCannon;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPointLightComponent* LightBusyAltCannon;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPointLightComponent* LightNotAmmoCannon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tank")
	float MoveSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tank")
	float RotationSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tank")
	float MoventSmooth = 0.01f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tank")
	float RotateSmooth = 0.01f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float TuretRotateSmooth = 0.01f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> DefaultCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> AlterCannonClass;
	


	//130631


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params", Meta = (MakeEditWidget = true))
	TArray<class ATargetPoint *> PatrollingPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Move params")
	float MovementAccuracy = 50.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent * SpringArm;

private:
	FTimerHandle DestroyTimerHandle;

	UPROPERTY()
	class ACannon* Cannon = nullptr;
	UPROPERTY()
	class ACannon* AlterCannon = nullptr;

	float CurentAxisMoveForward = 0.0f;
	float TargetAxisMoveForward = 0.0f;
	float TargetRotateRight = 0.0f;
	float CurentRotateRight = 0.0f;
	bool bIsSparks = false;
	bool bIsWorking = false;
	FVector TurretTargetPosition;
	FVector WorldMousePosition;
};
