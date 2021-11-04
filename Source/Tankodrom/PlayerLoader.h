// Fill out your copyright notice in the Description page of Project Settings.


//maps
#pragma once


#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerLoader.generated.h"

class ATankPawn;

UCLASS()
class TANKODROM_API ALoadPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY()
	ATankPawn* TankPawn;

	void Load();

};
