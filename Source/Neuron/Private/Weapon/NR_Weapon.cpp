// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NR_Weapon.h"
#include "Character/NR_Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"
#include "Components/ArrowComponent.h"
#include "Animation/AnimInstance.h"
#include "Weapon/NR_Projectile.h"
#include "Component/NR_InventoryComponent.h"

// Sets default values
ANR_Weapon::ANR_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create RootComponent
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	//Create Skeletal mesh
	SkeletalMeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshWeapon->SetGenerateOverlapEvents(false);
	SkeletalMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMeshWeapon->SetupAttachment(RootComponent);

	//Create Static mesh
	StaticMeshWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshWeapon->SetGenerateOverlapEvents(false);
	StaticMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshWeapon->SetupAttachment(RootComponent);

	//Create Shoot location
	ShootLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ShootLocation"));
	ShootLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANR_Weapon::BeginPlay()
{
	Super::BeginPlay();

	if (SkeletalMeshWeapon && !SkeletalMeshWeapon->GetSkeletalMeshAsset())
	{
		SkeletalMeshWeapon->DestroyComponent(true);
	}
	if (StaticMeshWeapon && !StaticMeshWeapon->GetStaticMesh())
	{
		StaticMeshWeapon->DestroyComponent(true);
	}
}

// Called every frame
void ANR_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireTimer += DeltaTime;

	ReloadTick(DeltaTime);
}

void ANR_Weapon::ReloadTick(float DeltaTime)
{
	if (WeaponReloading)
	{
		if (ReloadTimer <= 0.0f)
			EndReload();
		else
			ReloadTimer -= DeltaTime;
	}
}


int32 ANR_Weapon::GetWeaponRound()
{
	return WeaponInfo.Round;
}

FProjectileInfo ANR_Weapon::GetProjectile()
{
	return WeaponSetting.ProjectileSetting;
}

void ANR_Weapon::FireButtonPressed(bool bIsFire)
{
	if (bIsFire)
	{
		if (FireTimer >= WeaponRateOfFire)
		{
			Fire();
		}

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ANR_Weapon::Fire, WeaponRateOfFire, true, WeaponRateOfFire);
	}
	else
	{
			GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}
}

void ANR_Weapon::Fire()
{
	FireTimer = 0.0f;

	UAnimMontage* AnimMontage = nullptr;

	AnimMontage = WeaponSetting.AnimWeaponInfo.CharacterFireAnimMontage;

	uint8 NumberProjectile = WeaponSetting.NumberProjectileByShot;

	WeaponInfo.Round = WeaponInfo.Round - 1 * NumberProjectile;

	OnWeaponFireStart.Broadcast(AnimMontage);

	if (ShootLocation)
	{
		FVector SpawnLocation = ShootLocation->GetComponentLocation();
		FRotator SpawnRotation = ShootLocation->GetComponentRotation();

		FindEndLocation();

		FProjectileInfo ProjectileInfo;
		ProjectileInfo = GetProjectile();

		if (ProjectileInfo.Projectile)
		{

			float RotationToTarget = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, ShotEndLocation).Yaw;
			float RotationToTargetWithDispersion = RotationToTarget + UKismetMathLibrary::RandomFloatInRange(-CoefDispersion, CoefDispersion);

			if (WeaponSetting.AnimWeaponInfo.WeaponFireAnimMontage
				&& SkeletalMeshWeapon && SkeletalMeshWeapon->GetAnimInstance())
			{
				SkeletalMeshWeapon->GetAnimInstance()->Montage_Play(WeaponSetting.AnimWeaponInfo.WeaponFireAnimMontage);
			}

			int8 CoefConfusionShot = (NumberProjectile / 2) * -Confusion;
			FActorSpawnParameters SpawmParams;
			SpawmParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawmParams.Owner = GetOwner();
			SpawmParams.Instigator = GetInstigator();

			for (int8 i = 0; i < NumberProjectile; i++)
			{
				SpawnRotation = FRotator(0.0f, RotationToTargetWithDispersion + CoefDispersion * CoefConfusionShot, 0.0f);

				ANR_Projectile* Projectile = Cast<ANR_Projectile>(GetWorld()->SpawnActor(ProjectileInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawmParams));
				if (Projectile)
				{
					Projectile->InitProjectile(ProjectileInfo);
				}

				CoefConfusionShot = CoefConfusionShot + Confusion;
			}
		}
		else
		{
			FHitResult Hit;
			TArray<AActor*> Actors;

			UKismetSystemLibrary::LineTraceSingle(GetWorld(), SpawnLocation, SpawnLocation + ShootLocation->GetForwardVector() * WeaponSetting.LaserDistance,
				ETraceTypeQuery::TraceTypeQuery4, false, Actors, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Green, FLinearColor::Red, 5.0f);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponSetting.LaserFx, ShootLocation->GetComponentLocation(), UKismetMathLibrary::FindLookAtRotation(ShootLocation->GetComponentLocation(), ShootLocation->GetComponentLocation() + ShootLocation->GetForwardVector()));

			if (Hit.GetActor() && Hit.PhysMaterial.IsValid())
			{
				EPhysicalSurface SurfaceType = UGameplayStatics::GetSurfaceType(Hit);

				if (WeaponSetting.ProjectileSetting.HitDecal)
				{
					UMaterialInterface* Material = WeaponSetting.ProjectileSetting.HitDecal;

					if (Material && Hit.GetComponent())
					{
						UGameplayStatics::SpawnDecalAttached(Material, FVector(20.0f), Hit.GetComponent(), NAME_None, Hit.ImpactPoint,
							Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
					}
				}
				if (WeaponSetting.ProjectileSetting.HitFXs)
				{
					UParticleSystem* Particle = WeaponSetting.ProjectileSetting.HitFXs;

					if (Particle)
					{
						UGameplayStatics::SpawnEmitterAttached(Particle, Hit.GetComponent(), NAME_None, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(),
							EAttachLocation::KeepWorldPosition);
					}
				}
				if (WeaponSetting.ProjectileSetting.HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSetting.ProjectileSetting.HitSound, Hit.ImpactPoint, 0.3f);
				}

				UGameplayStatics::ApplyPointDamage(Hit.GetActor(), WeaponSetting.ProjectileSetting.ProjectileDamage, Hit.ImpactPoint, Hit, GetOwner()->GetInstigatorController(), this, nullptr);
			}
		}
	}

	if (GetWeaponRound() <= 0 && !WeaponReloading)
	{
		if (CheckWeaponReload())
		{
			StartReload();
		}
		else
		{
			Pawn->TrySwitchWeapon();
			Pawn->GetInventoryComponent()->OnAmmoTypeEmpty.Broadcast(WeaponSetting.WeaponType);
		}
	}
}

void ANR_Weapon::FindEndLocation()
{
	FVector DecalLocation = FMath::LinePlaneIntersection(Pawn->MouseLocation, Pawn->LineEnd, ShootLocation->GetComponentLocation(), FVector(0, 0, 1));

	FVector tmpV = (Pawn->GetActorLocation() - DecalLocation);

	if (tmpV.Size() > SizeChanelToChangeShootDirectionLogic)
	{
		ShotEndLocation = DecalLocation;
	}
	else
	{
		ShotEndLocation = ShootLocation->GetComponentLocation() + ShootLocation->GetForwardVector() * 2000.0f;
	}
}

void ANR_Weapon::StartReload()
{
	WeaponReloading = true;
	ReloadTimer = WeaponSetting.ReloadTime;

	UAnimMontage* AnimToPlay = WeaponSetting.AnimWeaponInfo.CharacterReloadAnimMontage;

	if (WeaponSetting.AnimWeaponInfo.WeaponReloadAnimMontage
		&& SkeletalMeshWeapon && SkeletalMeshWeapon->GetAnimInstance())
	{
		SkeletalMeshWeapon->GetAnimInstance()->Montage_Play(WeaponSetting.AnimWeaponInfo.WeaponReloadAnimMontage);
	}

	OnWeaponReloadStart.Broadcast(AnimToPlay);
}

void ANR_Weapon::EndReload()
{
	WeaponReloading = false;

	int8 AviableAmmoFromInventory = GetAviableAmmoForReload();

	if (AviableAmmoFromInventory > WeaponSetting.MaxRound)
		AviableAmmoFromInventory = WeaponSetting.MaxRound;

	const int32 AmmoTaken = WeaponSetting.MaxRound - WeaponInfo.Round;

	if (WeaponInfo.Round + AviableAmmoFromInventory > WeaponSetting.MaxRound)
	{
		WeaponInfo.Round = WeaponSetting.MaxRound;
	}
	else
	{
		WeaponInfo.Round = WeaponInfo.Round + AviableAmmoFromInventory;
	}

	OnWeaponReloadEnd.Broadcast(true, -AmmoTaken);
}

void ANR_Weapon::CancelReload()
{
	WeaponReloading = false;

	if (SkeletalMeshWeapon && SkeletalMeshWeapon->GetAnimInstance())
		SkeletalMeshWeapon->GetAnimInstance()->StopAllMontages(0.15f);

	OnWeaponReloadEnd.Broadcast(false, 0);
}

bool ANR_Weapon::CheckWeaponReload()
{
	bool result = true;

	if (GetOwner())
	{
		UNR_InventoryComponent* Inventory = Cast<UNR_InventoryComponent>(GetOwner()->GetComponentByClass(UNR_InventoryComponent::StaticClass()));
		if (Inventory)
		{
			int8 AviableAmmoForWeapon;

			if (!Inventory->CheckAmmoForWeapon(WeaponSetting.WeaponType, AviableAmmoForWeapon))
			{
				result = false;
			}
		}
	}
	return result;
}

int8 ANR_Weapon::GetAviableAmmoForReload()
{
	int8 AvailableAmmoForWeapon = 0;

	if (GetOwner())
	{
		UNR_InventoryComponent* myInventory = Cast<UNR_InventoryComponent>(GetOwner()->GetComponentByClass(UNR_InventoryComponent::StaticClass()));
		if (myInventory)
		{
			if (myInventory->CheckAmmoForWeapon(WeaponSetting.WeaponType, AvailableAmmoForWeapon))
			{
				AvailableAmmoForWeapon = AvailableAmmoForWeapon;
			}
		}
	}

	return AvailableAmmoForWeapon;
}

