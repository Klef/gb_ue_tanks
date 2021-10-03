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
		void MoveRight(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void RotateRight(float Value);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent * ArmorMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent * TurretMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 250.0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100.0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent * SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent * Camera;

	float TankAxisMoveForward = 0.0;
	float TankAxisMoveRight = 0.0;
	float TankRotateRight = 0.0;

};
