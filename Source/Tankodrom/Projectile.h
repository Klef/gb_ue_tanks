#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhisicMoventComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USoundBase* HitSoundEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float FireRange = 1000.0f;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	bool bIsPhisics = false;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPhisicMoventComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	bool bIsExplosion = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float ExplosionRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float ExplosionImpule = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float InTargetSpeed = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float AgilitySmooth = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	bool bIsRocket = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float Damage = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float Mass = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float EnableRange = 800;



public:
    AProjectile();

    void Start();
    void Stop();
	virtual void Tick(float DeltaTime) override;
	float GetMoveSpeed();
	UPhisicMoventComponent * GetMoveClass();

protected:
    UFUNCTION()
    virtual void OnMeshHit(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector  NormalImpulse, const FHitResult& HitResult);

	UFUNCTION()
	virtual void Explosion(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector  NormalImpulse, const FHitResult& HitResult);
private:
    FVector StarPosition;
	bool bIsRocketTarget = false;
	FVector Target = FVector::ZeroVector;

	
};
