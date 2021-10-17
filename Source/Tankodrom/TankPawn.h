// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"


class UStaticMeshComponent;
class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TANKODROM_API ATankPawn : public APawn
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
		void SetTurretTargetPosition(const FVector & TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void Fire();

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent * ArmorMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent * TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* CannonSpawnPoint;

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
		
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent * SpringArm;
private:
	void SetupCannon();

	UPROPERTY()
		class ACannon* Cannon = nullptr;

	float CurentAxisMoveForward = 0.0f;
	float TargetAxisMoveForward = 0.0f;
	float TargetRotateRight = 0.0f;
	float CurentRotateRight = 0.0f;
	FVector TurretTargetPosition;

};
