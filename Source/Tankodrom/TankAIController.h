// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankPawn.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class TANKODROM_API ATankAIController : public AAIController
{
	GENERATED_BODY()
protected:

	UPROPERTY()
	ATankPawn * TankPawn;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params", Meta = (MakeEditWidget = true))
// 	TArray<FVector> PatrollingPoints;

	int32 CurrentPatrolPointIndex = 0;
	TArray<FVector> PatrolPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accuracy = 10.0f;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params")
// 	float MovementAccuracy;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void MoveToNextPoint();
	void Targeting();
};
