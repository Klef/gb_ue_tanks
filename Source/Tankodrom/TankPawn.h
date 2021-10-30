// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Damageble.h"
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

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void Fire();
	UFUNCTION(BlueprintCallable, Category = "Turret")
		void FireSpecial();
	UFUNCTION(BlueprintCallable, Category = "Turret")
		void ReCharge();

	UFUNCTION(BlueprintPure, Category = "Turret")
		FVector GetTurretForwardVector();

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void ChangeCannon();

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void SetupCannon(TSubclassOf<class ACannon> InCannonClass);

	void AddAmmo(int32 CountAmmo);
	bool NullAmmo();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnHeathChange(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnDie();

	virtual void TakeDamage(const FDamageData& DamageData) override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	const TArray<FVector>& GetPatrolPoints()
	{
		return PatrollingPoint;
	};

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	float GetMovementAccuracy()
	{
		return MovementAccuracy;
	};

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent * ArmorMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent * TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
 	class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

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
	
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params", Meta = (MakeEditWidget = true))
	TArray<FVector> PatrollingPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Move params")
	float MovementAccuracy = 50.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent * SpringArm;

private:

	UPROPERTY()
	class ACannon* Cannon = nullptr;
	UPROPERTY()
	class ACannon* AltCannon = nullptr;

	float CurentAxisMoveForward = 0.0f;
	float TargetAxisMoveForward = 0.0f;
	float TargetRotateRight = 0.0f;
	float CurentRotateRight = 0.0f;
	FVector TurretTargetPosition;

};
