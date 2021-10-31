#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKODROM_API AProjectile : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UStaticMeshComponent* Mesh;

// 	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
// 	class UParticleSystemComponent* DestroyVisualEffect;
// 	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
// 	class UAudioComponent* DestroySoundEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UParticleSystem* HitVisualEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* FireVisualEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float FireRange = 1000.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float Damage = 1.0f;

public:
    AProjectile();

    void Start();
    virtual void Tick(float DeltaSeconds) override;

protected:
    UFUNCTION()
    void OnMeshHit(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector  NormalImpulse, const FHitResult& HitResult);
private:
    FVector StarPosition;
};
