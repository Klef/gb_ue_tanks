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
	//Three - Rocket
	//Four - Mortire
	CurentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, CurentLevel);
	NextLevel = TEXT("ZeroBrif");
	DeathLevel = TEXT("ZeroBrif");
	bool bIsDeath = false;
	if (CurentLevel == TEXT("ZeroBrif"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("Zero"));
		bIsLoadEnable = true;
		NextLevel = TEXT("FirstLevel");
		DeathLevel = TEXT("ZeroBrif");
	}
	if (CurentLevel == TEXT("FirstBrif"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("1"));
		bIsLoadEnable = true;
		bIsDeath = true;
		NextLevel = TEXT("FirstLevel");
		DeathLevel = TEXT("ZeroBrif");
	}
	if (CurentLevel == TEXT("TwoBrif"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("2"));
		bIsLoadEnable = true;
		bIsDeath = true;
		NextLevel = TEXT("TwoLevel");
		DeathLevel = TEXT("TwoBrif");
	}
	if (CurentLevel == TEXT("ThreeBrif"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("3"));
		bIsLoadEnable = true;
		bIsDeath = true;
		NextLevel = TEXT("ThreeLevel");
		DeathLevel = TEXT("ThreeBrif");
	}
	if (CurentLevel == TEXT("FourBrif"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("4"));
		bIsLoadEnable = true;
		bIsDeath = true;
		NextLevel = TEXT("FourLevel");
		DeathLevel = TEXT("FourBrif");
	}
	if (CurentLevel == TEXT("LastBrif"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("L"));
		bIsLoadEnable = true;
		NextLevel = TEXT("LastBrif");
		DeathLevel = TEXT("LastBrif");
	}
	if (CurentLevel == TEXT("FirstLevel"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("10"));
		NextLevel = TEXT("FirstLevel");
		DeathLevel = TEXT("FirstBrif");
	}
	if (CurentLevel == TEXT("TwoLevel"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("20"));
		NextLevel = TEXT("TwoLevel");
		DeathLevel = TEXT("TwoBrif");
	}
	if (CurentLevel == TEXT("ThreeLevel"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("30"));
		NextLevel = TEXT("ThreeLevel");
		DeathLevel = TEXT("ThreeBrif");
	}
	if (CurentLevel == TEXT("FourLevel"))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("40"));
		NextLevel = TEXT("FourLevel");
		DeathLevel = TEXT("FourBrif");
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("100"));
	TankPawn->SetDeathName(DeathLevel);
	TankPawn->SetLoaderBool(!bIsLoadEnable);
	if (bIsDeath)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("D"));
		TankPawn->SmokeSoundEffect->Play();
		TankPawn->SmokeVisualEffect->ActivateSystem();
		TankPawn->FireVisualEffect->ActivateSystem();
		TankPawn->FireSoundEffect->Play();
	}
	TankPawn->Load();
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
		UGameplayStatics::OpenLevel(GetWorld(), NextLevel);
	}
}


// void ATankPlayerController::ChangeCannon()
//{
//	if (TankPawn)
//	{
//		TankPawn->ChangeCannon();
//	}
//}