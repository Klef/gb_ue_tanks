// Fill out your copyright notice in the Description page of Project Settings.


#include "PhisicMoventComponent.h"

// Sets default values for this component's properties
UPhisicMoventComponent::UPhisicMoventComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UPhisicMoventComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPhisicMoventComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AActor* Owner = GetOwner();
	if (Owner)
	{
		FVector NewActorLocation = Owner->GetActorLocation() + Velocity * DeltaTime + Gravity * FMath::Square(DeltaTime) / 2.0f;
		Velocity += Gravity * DeltaTime;
		Owner->SetActorLocation(NewActorLocation, true);
	}
}

