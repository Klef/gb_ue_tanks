// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "Tankodrom.h"
#include "GameFramework/PlayerController.h"
#include "PlayerLoader.h"
#include "Kismet/GameplayStatics.h"

//UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);

void ALoadPlayerController::Load()
{
	FName LoadLevelName = TankPawn->GetLevelName();
	UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
}


void ALoadPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	bShowMouseCursor = true;
}

void ALoadPlayerController::SetupInputComponent()
{

}