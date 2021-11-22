// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	
}


void UHealthComponent::TakeDamage(const FDamageData& DamageData)
{
	float TakedDamageValue = DamageData.DamageValue;
	CurrentHealth -= TakedDamageValue;

	if (CurrentHealth <= 0.0f)
	{
		if (OnDie.IsBound())
			OnDie.Broadcast();
	}
	else
	{
		if (OnHeathChange.IsBound())
			OnHeathChange.Broadcast(TakedDamageValue);
	}
}

float UHealthComponent::GetHealht() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealhtState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::AddHealth(float AddiditionalHeatlhValue)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + AddiditionalHeatlhValue, 0.0f, MaxHealth);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

