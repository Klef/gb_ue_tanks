// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhisicMoventComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKODROM_API UPhisicMoventComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhisicMoventComponent();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
	FVector Gravity = FVector(0.0f, 0.0f, -980.0f);
	FVector Velocity = FVector::ZeroVector;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
