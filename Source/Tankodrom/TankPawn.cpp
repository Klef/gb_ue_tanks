// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewLocation = GetActorLocation() + MoveSpeed * (GetActorForwardVector() * TankAxisMoveForward + GetActorRightVector() * TankAxisMoveRight) * DeltaTime;
	
	SetActorLocation(NewLocation);

	float NewYawRotation = GetActorRotation().Yaw + TankRotateRight * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0, NewYawRotation, 0));
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::MoveForward(float AxisValue)
{
	TankAxisMoveForward = AxisValue;
}

void ATankPawn::MoveRight(float AxisValue)
{
	TankAxisMoveRight = AxisValue;
}

void ATankPawn::RotateRight(float Value)
{
	TankRotateRight = Value;
}