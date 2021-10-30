// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Tankodrom.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

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

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(ArmorMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHeathChange.AddDynamic(this, &ATankPawn::OnHeathChange);
	HealthComponent->OnDie.AddDynamic(this, &ATankPawn::OnDie);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon(DefaultCannonClass);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//плавность от момента времени
	//CurentAxisMoveForward = FMath::Lerp(CurentAxisMoveForward, TargetAxisMoveForward, MoventSmooth);

	CurentAxisMoveForward = FMath::FInterpTo(CurentAxisMoveForward, TargetAxisMoveForward, DeltaTime, MoventSmooth);
	FVector MoveVector = GetActorForwardVector() * CurentAxisMoveForward;
	FVector NewLocation = GetActorLocation() + MoveVector * MoveSpeed * DeltaTime;
	SetActorLocation(NewLocation, true);
	
	
	//CurentRotateRight = FMath::Lerp(CurentRotateRight, TargetRotateRight, RotateSmooth);
	CurentRotateRight = FMath::FInterpTo(CurentRotateRight, TargetRotateRight, DeltaTime, RotateSmooth);
	float NewYawRotation = GetActorRotation().Yaw + CurentRotateRight * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0, NewYawRotation, 0));
// 	
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Blue, TEXT("STUCK"));
// 	}
	//
	// 
	//example log in debug
	//UE_LOG(LogTank, Verbose, TEXT("CurentRotateRight: %f"), CurentRotateRight);
	//UE_LOG(LogTank, Verbose, TEXT("TargetRotateRight: %f"), TargetRotateRight);

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetPosition);
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Roll = CurrentRotation.Roll;
	TargetRotation.Pitch = CurrentRotation.Pitch;
	//TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TuretRotateSmooth));
	TurretMesh->SetWorldRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TuretRotateSmooth));

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

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::ReCharge()
{
	if (Cannon)
	{
		Cannon->ReCharge();
	}
}


void ATankPawn::ChangeCannon()
{
	if (Cannon)
	{
		if (AltCannon)
		{
			Swap(Cannon, AltCannon);
			Cannon->SetVisibility(true);
			AltCannon->SetVisibility(false);
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("Change Weapon"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, TEXT("No Secondary Weapon"));
		}
	}
}

void ATankPawn::SetupCannon(TSubclassOf<class ACannon> InCannonClass)
{
	if (!AltCannon && Cannon)
	{
		AltCannon = Cannon;
	}
	else
	{
		if (Cannon)
		{
			Cannon->Destroy();
		}
	}
	if (InCannonClass)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, Params);
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ATankPawn::AddAmmo(int32 CountAmmo)
{
	if (Cannon)
	{
		Cannon->AddAmmo(CountAmmo);
	}
}

bool ATankPawn::NullAmmo()
{
	if (Cannon)
	{
		return Cannon->NullAmmo();
	}
	else
	{
		return false;
	}
}

void ATankPawn::Destroyed()
{
	Super::Destroyed();
	if (Cannon)
	{
		Cannon->Destroy();
	}
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::OnHeathChange_Implementation(float Damage)
{

}

void ATankPawn::OnDie_Implementation()
{
	Destroy();
}

void ATankPawn::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}