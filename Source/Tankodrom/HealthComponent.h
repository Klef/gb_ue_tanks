// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStructs.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float, DamageValue);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKODROM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health values")
	float MaxHealth = 10.0f;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHeathChange;

	UPROPERTY(BlueprintAssignable)
	FOnDie OnDie;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void TakeDamage(const FDamageData& DamageData);

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	float GetHealht() const;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	float GetHealhtState() const;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void AddHealth(float AddiditionalHeatlhValue);

protected:
	UPROPERTY()
	float CurrentHealth = 0.0f;

	virtual void BeginPlay() override;

};
