// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"


UCLASS()
class TANKODROM_API AAmmoBox : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	EAmmoBoxType TypeBox = EAmmoBoxType::BoxCannon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditComdition = "Type == EAmmoBoxType::BoxCannon;", EditConditionHides), Category = "Ammo")
	TSubclassOf<class ACannon> CannonClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditComdition = "Type == EAmmoBoxType::BoxAmmo;", EditConditionHides), Category = "Ammo")
	int32 AmmoInBox = 10;
public:
	AAmmoBox();

protected:

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
