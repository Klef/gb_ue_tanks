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
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "Tankodrom.h"

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

	AlterTurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Alter Cannon"));
	AlterTurretMesh->SetupAttachment(TurretMesh);

	CannonAlterSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Alter point"));
	CannonAlterSpawnPoint->SetupAttachment(AlterTurretMesh);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	CannonSpawnPoint->SetupAttachment(TurretMesh);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(ArmorMesh);

	HitTurretCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Turret collider"));
	HitTurretCollider->SetupAttachment(TurretMesh);

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
	//	ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated lights"));
	//ActivatedLight->SetupAttachment(SceneComp);
	LightReadyCannon = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Cannon Ready"));
	LightReadyCannon->SetupAttachment(ArmorMesh);
	LightReadyAltCannon = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Alter Cannon Ready"));
	LightReadyAltCannon->SetupAttachment(ArmorMesh);
	LightBusyCannon = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Cannon Busy"));
	LightBusyCannon->SetupAttachment(ArmorMesh);
	LightBusyAltCannon = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Alter Cannon Busy"));
	LightBusyAltCannon->SetupAttachment(ArmorMesh);
	LightNotAmmoCannon = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Cannon Not Ammo"));
	LightNotAmmoCannon->SetupAttachment(ArmorMesh);
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
	SetupAlterCanon(AlterCannonClass);
	Load();
// -1634,0
// 558,0

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
	UE_LOG(LogTank, Verbose, TEXT("MoveForward: %f, %f"), CurentAxisMoveForward, TargetAxisMoveForward);

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

	if (Cannon)
	{
		if (Cannon->NullAmmo())
		{
			LightReadyCannon->SetHiddenInGame(true);
			LightBusyCannon->SetHiddenInGame(true);
			LightNotAmmoCannon->SetHiddenInGame(false);
		}
		else
		{
			if (Cannon->IsReadyToFire())
			{
				LightReadyCannon->SetHiddenInGame(false);
				LightBusyCannon->SetHiddenInGame(true);
				LightNotAmmoCannon->SetHiddenInGame(true);
			}
			else
			{
				LightReadyCannon->SetHiddenInGame(true);
				LightBusyCannon->SetHiddenInGame(false);
				LightNotAmmoCannon->SetHiddenInGame(true);
			}
		}
	}
	if (AlterCannon)
	{

		if (AlterCannon->IsReadyToFire())
		{
			LightReadyAltCannon->SetHiddenInGame(false);
			LightBusyAltCannon->SetHiddenInGame(true);
		}
		else
		{
			LightReadyAltCannon->SetHiddenInGame(true);
			LightBusyAltCannon->SetHiddenInGame(false);
		}
	
		
	}
	else
	{
		LightReadyAltCannon->SetHiddenInGame(true);
		LightBusyAltCannon->SetHiddenInGame(true);
	}

	CurentRotateRight = FMath::Lerp(CurentRotateRight, TargetRotateRight, RotateSmooth);
	//CurentRotateRight = FMath::FInterpTo(CurentRotateRight, TargetRotateRight, DeltaTime, RotateSmooth);
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
	if (AlterCannon)
	{
		if (AlterCannon->isMortable())
		{
			//DrawDebugLine(GetWorld(), TankPawn->GetActorLocation(), TurretTargetPosition, FColor::White, false, 0.1f, 0, 5.f);
			//FVector::DistSquared
			//if (FVector::DistSquared(PlayerPawn->GetActorLocation(), TankPawn->GetActorLocation()) > FMath::Square(TargetingRange))
			//Projectile* DefaultProjectile = ProjectileClass->GetDefaultObject<APhysicsProjectile>();
			//TSubclassOf<class AProjectile> DefaultProjectileClass = AlterCannon->GetProjectileClass();
			AProjectile* DefaultProjectile = AlterCannon->GetProjectileClass()->GetDefaultObject<AProjectile>();
			check(DefaultProjectile);
			FVector Local = GetActorLocation();
			float CurrentDist = FVector::Dist2D(GetActorLocation(), WorldMousePosition);
			float Speed = DefaultProjectile->GetMoveSpeed();
			float Gravity = - DefaultProjectile->GetMoveClass()->Gravity.Z;
			if (FMath::IsNearlyZero(Gravity))
			{
				// Can't calculate
				return;
			}

			float Z = AlterCannon->GetZSpawn() - WorldMousePosition.Z; // высота точки выстрела - точка прицеливания, упростим
			float X = CurrentDist;
			float Angle = 85.f; // небеса
			if (!FMath::IsNearlyZero(X))
			{
				float FirstSqrtComp = FMath::Pow(Speed, 4);
				float SecondSqrtComp = Gravity * (Gravity * FMath::Square(X) - 2 * (FMath::Square(Speed) * Z));
				float SqrtComp = 0.f;
				if (FirstSqrtComp > SecondSqrtComp)
				{
					SqrtComp = FMath::Sqrt(FirstSqrtComp - SecondSqrtComp);
				}
				if (AlterCannon->GetIsSmallAgile())
				{
					Angle = FMath::Atan((FMath::Square(Speed) - SqrtComp) / (Gravity * X));
				}
				else
				{
					Angle = FMath::Atan((FMath::Square(Speed) + SqrtComp) / (Gravity * X));
				}
				Angle = FMath::RadiansToDegrees(Angle);
				UE_LOG(LogTank, Verbose, TEXT("Dist %f, Angle %f "), CurrentDist, Angle);

				
			}


			//DrawDebugLine(GetWorld(), WorldMousePosition, GetActorLocation(), FColor::Green, false, 0.1f, 0, 5.f);
			FRotator AlterTargetRotation = FRotator::ZeroRotator;
 			FRotator CurrenAlterRotation = AlterCannon->GetActorRotation();
			AlterTargetRotation.Roll = CurrenAlterRotation.Roll;
			AlterTargetRotation.Pitch = Angle;
			AlterTargetRotation.Yaw = CurrenAlterRotation.Yaw;
			//
			//TurretMesh->SetWorldRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TuretRotateSmooth));
			//
			//AlterCannon->SetActorRotation(FMath::Lerp(CurrenAlterRotation, AlterTargetRotation, TuretRotateSmooth));
			AlterCannon->SetActorRotation(FMath::RInterpTo(CurrenAlterRotation, AlterTargetRotation, DeltaTime, TuretRotateSmooth));
			float Ag = Angle;
		}
	}

}



void ATankPawn::SetTurretTargetPosition(const FVector& TargetPosition)
{
	TurretTargetPosition = TargetPosition;
}



void ATankPawn::SetMousePosition(FVector MouseValue)
{
	WorldMousePosition = MouseValue;
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
	if (AlterCannon)
	{
		AlterCannon->Fire();
	}
}

void ATankPawn::ReCharge()
{
	if (Cannon)
	{
		Cannon->ReCharge();
	}
}


// void ATankPawn::ChangeCannon()
// {
// 	if (Cannon)
// 	{
// 		if (AltCannon)
// 		{
// 			Swap(Cannon, AltCannon);
// 			Cannon->SetVisibility(true);
// 			AltCannon->SetVisibility(false);
// 			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("Change Weapon"));
// 		}
// 		else
// 		{
// 			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, TEXT("No Secondary Weapon"));
// 		}
// 	}
// }

void ATankPawn::SetupCannon(TSubclassOf<class ACannon> InCannonClass)
{
	if (Cannon)
	{
		Cannon->Destroy();
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
void ATankPawn::SetupAlterCanon(TSubclassOf<class ACannon> InCannonClass)
{
	if (AlterCannon)
	{
		AlterCannon->Destroy();
	}
	if (InCannonClass)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		AlterCannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, Params);
		AlterCannon->AttachToComponent(CannonAlterSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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
	if (AlterCannon)
	{
		AlterCannon->Destroy();
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

	HitEffect();
}

void ATankPawn::OnDie_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, TEXT("Destroy"));
	//DestroyVisualEffect->ActivateSystem();
	//DestroySoundEffect->Play();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyVisualEffect, GetActorTransform().GetLocation(), GetActorTransform().GetRotation().Rotator(), FVector(3.0, 3.0, 3.0), true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySoundEffect, GetActorLocation());
	//GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ATankPawn::DestroyWait, 0.5f, false);
	if (this == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		UGameplayStatics::OpenLevel(GetWorld(), DeathLevel);
	}
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

void ATankPawn::HitEffect()
{
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

void ATankPawn::Load()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("Preved"));
	if (!bIsInput)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::White, TEXT("UPS"));
		SpringArm->SetWorldRotation(FRotator(180.0f, 0.0f, 180.0f));
		SpringArm->SetWorldLocation(FVector(-4800.0f, -10.0f, 190.0f));
	}
}
