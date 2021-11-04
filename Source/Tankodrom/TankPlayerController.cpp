// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "Tankodrom.h"
#include "Kismet/GameplayStatics.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	bShowMouseCursor = true;

	CurentLevel = GetWorld()->GetMapName();
	//First - solo
	//Two - boorst
	//Three - Laser
	//Five - Rocket
	//Six - Mortire
	NextLevel = "FirstBrif";
	if (CurentLevel == TEXT("UEDPIE_0_ZeroBrif"))
	{
		bIsLoadEnable = true;
		NextLevel = TEXT("FirstLevel");
		DeathLevel = TEXT("ZeroBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_FirstBrif"))
	{
		bIsLoadEnable = true;
		NextLevel = TEXT("TwoLevel");
		DeathLevel = TEXT("FirstBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_TwoBrif"))
	{
		bIsLoadEnable = true;
		NextLevel = TEXT("ThreeLevel");
		DeathLevel = TEXT("TwoBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_ThreeBrif"))
	{
		bIsLoadEnable = true;
		NextLevel = TEXT("FourLevel");
		DeathLevel = TEXT("ThreeBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_FourBrif"))
	{
		bIsLoadEnable = true;
		NextLevel = TEXT("FiveLevel");
		DeathLevel = TEXT("FourBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_FiveBrifl"))
	{
		bIsLoadEnable = true;
		NextLevel = TEXT("SixLevel");
		DeathLevel = TEXT("FiveBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_SixBrif"))
	{
		bIsLoadEnable = true;
		NextLevel = TEXT("LastBrif");
		DeathLevel = TEXT("SixBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_LastBrif"))
	{
		NextLevel = TEXT("LastBrif");
		DeathLevel = TEXT("LastBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_FirstLevel"))
	{
		NextLevel = TEXT("FirstLevel");
		DeathLevel = TEXT("FirstBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_TwoLevel"))
	{
		NextLevel = TEXT("TwoLevel");
		DeathLevel = TEXT("TwoBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_ThreeLevel"))
	{
		NextLevel = TEXT("ThreeLevel");
		DeathLevel = TEXT("ThreeBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_FourLevel"))
	{
		NextLevel = TEXT("FourLevel");
		DeathLevel = TEXT("FourBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_FiveLevel"))
	{
		NextLevel = TEXT("FiveLevel");
		DeathLevel = TEXT("FiveBrif");
	}
	if (CurentLevel == TEXT("UEDPIE_0_SixLevel"))
	{
		NextLevel = TEXT("SixLevel");
		DeathLevel = TEXT("SixBrif");
	}

	TankPawn->DeathLevel = DeathLevel;
	TankPawn->bIsInput = !bIsLoadEnable;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, & ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, & ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, & ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("ReCharge", IE_Pressed, this, &ATankPlayerController::ReCharge);
	//InputComponent->BindAction("ChangeCannon", IE_Pressed, this, &ATankPlayerController::ChangeCannon);

	InputComponent->BindAction("Load", IE_Pressed, this, &ATankPlayerController::Load);


}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn && !bIsLoadEnable)
	{
		TankPawn->MoveForward(AxisValue);
	}
}


void ATankPlayerController::RotateRight(float Value)
{
	if (TankPawn && !bIsLoadEnable)
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
		TurretTargetDirection.Z = 21.0f;
		TurretTargetDirection.Normalize();
		FVector TurretTargetPosition = TankPawn->GetActorLocation() + TurretTargetDirection * 1000.0f;
		TankPawn->SetTurretTargetPosition(TurretTargetPosition);

		float DistanceAboveGround = 21.0f; //ground, bad haedcore
		FVector PlaneOrigin(0.0f, 0.0f, DistanceAboveGround);

		FVector MouseWorldLocation = FMath::LinePlaneIntersection(
			WorldMousePosition,
			WorldMousePosition + WorldMouseDirection,
			PlaneOrigin,
			FVector::UpVector);
		//DrawDebugLine(GetWorld(), MouseWorldLocation, TankPawn->GetActorLocation(), FColor::Green, false, 0.1f, 0, 5.f);
		TankPawn->SetMousePosition(MouseWorldLocation);
		
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn && !bIsLoadEnable)
	{
		TankPawn->Fire();
	}
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn && !bIsLoadEnable)
	{
		TankPawn->FireSpecial();
	}
}

void ATankPlayerController::ReCharge()
{
	if (TankPawn && !bIsLoadEnable)
	{
		TankPawn->ReCharge();
	}
}

void ATankPlayerController::Load()
{

	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, NextLevel);
	if (bIsLoadEnable)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(NextLevel));
	}
}


// void ATankPlayerController::ChangeCannon()
//{
//	if (TankPawn)
//	{
//		TankPawn->ChangeCannon();
//	}
//}