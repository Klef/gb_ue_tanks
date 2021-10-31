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
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

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

// 	HitVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Hit Effect"));
// 	HitVisualEffect->SetupAttachment(ArmorMesh);

//  	DestroyVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Destroy Effect"));
//  	DestroyVisualEffect->SetupAttachment(ArmorMesh);

	SmokeVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Effect"));
	SmokeVisualEffect->SetupAttachment(ArmorMesh);

	FireVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Effect"));
	FireVisualEffect->SetupAttachment(ArmorMesh);

	SparksVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Sparks Effect"));
	SparksVisualEffect->SetupAttachment(ArmorMesh);

	EngineVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Engine Effect"));
	EngineVisualEffect->SetupAttachment(ArmorMesh);


//	HitSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio HIt Effect"));
//	HitSoundEffect->SetupAttachment(ArmorMesh);

// 	DestroySoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Destroy Effect"));
// 	DestroySoundEffect->SetupAttachment(ArmorMesh);

	SmokeSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Smoke Effect"));
	SmokeSoundEffect->SetupAttachment(ArmorMesh);

	FireSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Fire Effect"));
	FireSoundEffect->SetupAttachment(ArmorMesh);

	SparksSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Sparks Effect"));
	SparksSoundEffect->SetupAttachment(ArmorMesh);

	EngineSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Sound Effect"));
	EngineSoundEffect->SetupAttachment(ArmorMesh);
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

	if (Cannon->isMortable())
	{
		//DrawDebugLine(GetWorld(), TankPawn->GetActorLocation(), TurretTargetPosition, FColor::White, false, 0.1f, 0, 5.f);
		//FVector::DistSquared
		//if (FVector::DistSquared(PlayerPawn->GetActorLocation(), TankPawn->GetActorLocation()) > FMath::Square(TargetingRange))
		float CurrentDist = FVector::DistSquared(GetActorLocation(), TurretTargetPosition);
		TargetRotation.Yaw = CurrentRotation.Yaw;

	}

	if (bIsWorking && TargetAxisMoveForward == 0)
	{
		bIsWorking = false;
		EngineSoundEffect->Stop();
		EngineVisualEffect->DeactivateSystem();
	}
	if (!bIsWorking && TargetAxisMoveForward == 1)
	{
		bIsWorking = true;
		EngineSoundEffect->Play();
		EngineVisualEffect->ActivateSystem();
	}
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

// void ATankPawn::DestroyWait()
// {
// 	//DestroyVisualEffect->ActivateSystem();
// 	//GetWorld()->SpawnActor<BP_TestAddAmmoBox>
// 	Destroy();
// }

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::OnHeathChange_Implementation(float Damage)
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, TEXT("HIT"));
	//HitVisualEffect->ActivateSystem();
	//HitSoundEffect->Play();
	if (!bIsFiring && HealthComponent->GetHealhtState() < 0.4f)
	{
		bIsFiring = true;
		FireVisualEffect->ActivateSystem();
		FireSoundEffect->Play();
	}
	if (!bIsSmoking && HealthComponent->GetHealhtState() < 0.8f)
	{
		bIsSmoking = true;
		SmokeSoundEffect->Play();
		SmokeVisualEffect->ActivateSystem();
	}
	if (!bIsSparks && HealthComponent->GetHealhtState() < 0.1f)
	{
		bIsSparks = true;
		SparksSoundEffect->Play();
		SparksVisualEffect->ActivateSystem();
	}
}

void ATankPawn::OnDie_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, TEXT("Destroy"));
	//DestroyVisualEffect->ActivateSystem();
	//DestroySoundEffect->Play();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyVisualEffect, GetActorTransform().GetLocation(), GetActorTransform().GetRotation().Rotator(), FVector(3.0, 3.0, 3.0), true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySoundEffect, GetActorLocation());
	//GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ATankPawn::DestroyWait, 0.5f, false);

	Destroy();
}

// void ATankPawn::EndPlay(EEndPlayReason::Type EndPlayReason)
// {
// 	Super::EndPlay(EndPlayReason);
// 	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
// }

void ATankPawn::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}