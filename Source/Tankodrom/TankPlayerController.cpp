// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "Tankodrom.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	bShowMouseCursor = true;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, & ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, & ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, & ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("ReCharge", IE_Pressed, this, &ATankPlayerController::ReCharge);
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(AxisValue);
	}
}


void ATankPlayerController::RotateRight(float Value)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(Value);
	}
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (TankPawn)
	{
		FVector WorldMousePosition, WorldMouseDirection;
		DeprojectMousePositionToWorld(WorldMousePosition, WorldMouseDirection);

		FVector TurretTargetDirection = WorldMousePosition - TankPawn->GetActorLocation();
		TurretTargetDirection.Z = 0.0f;
		TurretTargetDirection.Normalize();
		FVector TurretTargetPosition = TankPawn->GetActorLocation() + TurretTargetDirection * 1000.0f;
		TankPawn->SetTurretTargetPosition(TurretTargetPosition);
		DrawDebugLine(GetWorld(), TankPawn->GetActorLocation(), TurretTargetPosition, FColor::Green, false, 0.1f, 0, 5.f);


	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
	{
		TankPawn->FireSpecial();
	}
}

void ATankPlayerController::ReCharge()
{
	if (TankPawn)
	{
		TankPawn->ReCharge();
	}
}