// Fill out your copyright notice in the Description page of Project Settings.


//?????? ?????
//?????? ???????????
//?????? ?????
//??????? ?? ?????
//?????????? 

//maps
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

UCLASS()
class TANKODROM_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
		ATankPawn * TankPawn;

	void MoveForward(float AxisValue);
	void RotateRight(float Value);
	void Fire();
	void FireSpecial();
	//void ChangeCannon();
	void ReCharge();
	void Load();

	FName NextLevel;
	FName DeathLevel;
	FString CurentLevel;
	bool bIsInput = true;
	bool bIsLoadEnable = false;
};
