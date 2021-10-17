// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Tankodrom.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ArmorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = ArmorMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(ArmorMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(ArmorMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	CannonSpawnPoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurentAxisMoveForward = FMath::Lerp(CurentAxisMoveForward, TargetAxisMoveForward, MoventSmooth);
	FVector MoveVector = GetActorForwardVector() * CurentAxisMoveForward;
	FVector NewLocation = GetActorLocation() + MoveVector * MoveSpeed * DeltaTime;
	
	SetActorLocation(NewLocation);
	CurentRotateRight = FMath::Lerp(CurentRotateRight, TargetRotateRight, RotateSmooth);
	float NewYawRotation = GetActorRotation().Yaw + CurentRotateRight * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0, NewYawRotation, 0));
	//example log in debug
	//UE_LOG(LogTank, Verbose, TEXT("CurentRotateRight: %f"), CurentRotateRight);
	//UE_LOG(LogTank, Verbose, TEXT("TargetRotateRight: %f"), TargetRotateRight);

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetPosition);
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Roll = CurrentRotation.Roll;
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TuretRotateSmooth));

}


void ATankPawn::SetTurretTargetPosition(const FVector& TargetPosition)
{
	TurretTargetPosition = TargetPosition;
}



void ATankPawn::MoveForward(float AxisValue)
{
	TargetAxisMoveForward = AxisValue;
}

//void ATankPawn::MoveRight(float AxisValue)
//{
//	TankAxisMoveRight = AxisValue;
//}

void ATankPawn::RotateRight(float Value)
{
	TargetRotateRight = Value;
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::SetupCannon()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(DefaultCannonClass, Params);
	Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}