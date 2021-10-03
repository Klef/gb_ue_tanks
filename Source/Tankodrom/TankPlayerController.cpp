// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, & ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, & ATankPlayerController::MoveRight);
	InputComponent->BindAxis("RotateRight", this, & ATankPlayerController::RotateRight);
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(AxisValue);
	}
}

void ATankPlayerController::MoveRight(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveRight(AxisValue);
	}
}

void ATankPlayerController::RotateRight(float Value)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(Value);
	}
}