// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "Components/StaticMeshComponent.h"
#include "TankPawn.h"


AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled((ECollisionEnabled::QueryAndPhysics));
	Mesh->SetGenerateOverlapEvents(true);
}

void AAmmoBox::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (OtherActor == PlayerPawn)
	{
		if (TypeBox == EAmmoBoxType::BoxCannon)
		{
			PlayerPawn->SetupCannon(CannonClass);
		}
		if (TypeBox == EAmmoBoxType::BoxAmmo)
		{
			PlayerPawn->AddAmmo(AmmoInBox);
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, TEXT("ADD AMMO"));
		}
		Destroy();
	}
}
