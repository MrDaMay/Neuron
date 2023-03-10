// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Type.generated.h"

// enumeration for weapon
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	RifleType UMETA(DisplayName = "Rifle"),
	GunType UMETA(DisplayName = "Gun"),
	GrenadeType UMETA(DisplayName = "Grende"),
	LaserType UMETA(DisplayName = "Laser"),
};

// enumeration for tokens // ** Used for an array indexing, do NOT change! ** //
UENUM(BlueprintType)
enum class ETokenType : uint8
{
	DAMAGE,
	HEALTH,
	FIRESPEED,
	WALKSPEED,
	MIGHT,
	LUCK,
	NUMBER_OF_TOKENS,
};

//Information about projectile. Link, meshes, emmiters, damages, speed
USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		TSubclassOf<class ANR_Projectile> Projectile = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		UStaticMesh* ProjectileStaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		FTransform ProjectileStaticMeshOffset = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		UParticleSystem* ProjectileTrailFx = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		FTransform ProjectileTrailFxOffset = FTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		int32 Penetration = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		float ProjectileDamage = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		float ProjectileLifeTime = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		float ProjectInitSpeed = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		float ProjectMaxSpeed = 1000.0f;

	//Hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
		UMaterialInterface* HitDecal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
		USoundBase* HitSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
		UParticleSystem* HitFXs;

	//Explose
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explose")
		UParticleSystem* ExploseFX = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explose")
		USoundBase* ExploseSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explose")
		float ProjectileMaxRadiusDamage = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explose")
		float ExploseMaxDamage = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explose")
		float TimeToExplose = 0.5f;
};

//Animation for character and weapon
USTRUCT(BlueprintType)
struct FAnimWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		UAnimMontage* CharacterFireAnimMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		UAnimMontage* WeaponFireAnimMontage = nullptr;
};

//Information about weapon. Link, meshes, emmiters, damages, speed
USTRUCT(BlueprintType)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
		TSubclassOf<class ANR_Weapon> WeaponClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float RateOfFire = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		int32 NumberProjectileByShot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
		float DispersionWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FAnimWeaponInfo AnimWeaponInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		FProjectileInfo ProjectileSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
		float LaserDamage = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
		float LaserDistance = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
		UParticleSystem* LaserFx = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
		float DelayLaserForFire = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
		UTexture2D* WeaponIcon;
};

//Connect enemy with his name
USTRUCT(BlueprintType)
struct FEnemyCharacters : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		FName EnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		TSubclassOf<class ANR_EnemyCharacterBase> Enemy = nullptr;

};

USTRUCT(BlueprintType)
struct FAchievementLevel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		float Multiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		UTexture2D* Texture;
};

USTRUCT(BlueprintType)
struct FAchivementsInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
		FName AchievementName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
		TArray<FAchievementLevel> Levels;
};

USTRUCT(BlueprintType)
struct FCharStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CoefMovementSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CoefFireSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CoefDamage = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CoefDamageResist = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool Immortality = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float BaseSpeed = 600.0f;

	FCharStats& operator= (const FCharStats& Stats)
	{
		CoefMovementSpeed = Stats.CoefMovementSpeed;
		CoefFireSpeed = Stats.CoefFireSpeed;
		CoefDamage = Stats.CoefDamage;
		CoefDamageResist = Stats.CoefDamageResist;
		Immortality = Stats.Immortality;
		BaseSpeed = Stats.BaseSpeed;
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FDropObjects: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
		TSubclassOf<class AActor> DropItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
		UStaticMesh* DropMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
		UParticleSystem* LightPartical = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
		UParticleSystem* OverlapPartical = nullptr;
};

UCLASS()
class NEURON_API UType : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
