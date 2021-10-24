#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
    FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
    FireTrace = 1  UMETA(DisplayName = "Use trace")
};

UENUM(BlueprintType)
enum class EAmmoBoxType : uint8
{
    BoxAmmo = 0 UMETA(DisplayName = "Ammo Box"),
    BoxCannon = 1 UMETA(DisplayName = "Cannon Box")
};