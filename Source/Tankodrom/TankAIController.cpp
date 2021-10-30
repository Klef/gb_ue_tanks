// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "DrawDebugHelpers.h"
#include "Tankodrom.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
	if (TankPawn)
	{
		UE_LOG(LogTank, Verbose, TEXT("T %f %f. "), TankPawn->GetActorLocation().X, TankPawn->GetActorLocation().Y);
		for (const FVector& Point : TankPawn->GetPatrolPoints())
		{
			UE_LOG(LogTank, Verbose, TEXT("L %f %f. "), Point.X, Point.Y);
			UE_LOG(LogTank, Verbose, TEXT("W %f %f. "), TankPawn->GetActorLocation().X + Point.X, TankPawn->GetActorLocation().Y + Point.Y);
			PatrolPoint.Add(TankPawn->GetActorLocation() + Point);
		}
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TankPawn)
	{
		MoveToNextPoint();
		Targeting();
	}
}

void ATankAIController::MoveToNextPoint()
{
	if (PatrolPoint.Num() == 0)
	{
		return;
	}
	TankPawn->MoveForward(1.f);
	
	FVector PawnLocation = TankPawn->GetActorLocation();
	FVector CurrentPoint = PatrolPoint[CurrentPatrolPointIndex];
	if (FVector::DistSquared(PawnLocation, CurrentPoint) <= FMath::Square(TankPawn->GetMovementAccuracy()))
	{
		CurrentPatrolPointIndex++;
	}
	if (!PatrolPoint.IsValidIndex(CurrentPatrolPointIndex))
	{
		CurrentPatrolPointIndex = 0;
	}

	FVector moveDirection = CurrentPoint - PawnLocation;
	moveDirection.Normalize();
	FVector ForwardDirection = TankPawn->GetActorForwardVector();
	FVector RightDirection = TankPawn->GetActorRightVector();

	//DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);
	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, moveDirection)));
	float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, moveDirection)));

	float RotationValue = 0.0f;
	if (ForwardAngle > 5.0f)
		RotationValue = 1;
	if (RightAngle > 90.0f)
		RotationValue = - RotationValue;

	//UE_LOG(LogTemp, Warning, TEXT("AI Rotation forwardAngle: %f rightAngle: %f rotationValue: %f"), forwardAngle, rightAngle, rotationValue);
	TankPawn->RotateRight(RotationValue);

}

void ATankAIController::Targeting()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
	{
		return;
	}
	if (FVector::DistSquared(PlayerPawn->GetActorLocation(), TankPawn->GetActorLocation()) > FMath::Square(TargetingRange))
	{
		return;
	}
	FHitResult HitResult;
	FVector TraceStart = TankPawn->GetActorLocation();
	FVector TraceEnd = PlayerPawn->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("AI Vision Trace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
	{
		if (HitResult.Actor != PlayerPawn)
		{
			return;
		}
	}

	TankPawn->SetTurretTargetPosition(PlayerPawn->GetActorLocation());

	FVector TargetingDir = TankPawn->GetTurretForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	DirToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
	
	if (AimAngle <= Accuracy)
	{
		TankPawn->Fire();
		if (TankPawn->NullAmmo())
		{
			TankPawn->ReCharge();
		}
	}
	//1-17
}