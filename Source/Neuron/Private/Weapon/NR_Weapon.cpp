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

	FireTimer = 100.0f;
}

// Called every frame
void ANR_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireTimer += DeltaTime;
}

FProjectileInfo ANR_Weapon::GetProjectile()
{
	return WeaponSetting.ProjectileSetting;
}

void ANR_Weapon::FireButtonPressed(bool bIsFire)
{
	if (bIsFire)
	{
		if (GetProjectile().Projectile)
		{
			if (FireTimer >= WeaponSetting.RateOfFire)
			{
				FireProjectile();
			}

			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ANR_Weapon::FireProjectile, WeaponSetting.RateOfFire, true, WeaponSetting.RateOfFire);
		}
		else
			GetWorldTimerManager().SetTimer(LaserFireTimerHandle, this, &ANR_Weapon::LaserFire, WeaponSetting.DelayLaserForFire, true, WeaponSetting.DelayLaserForFire);
	}
	else
	{
		if (GetWorldTimerManager().IsTimerActive(LaserFireTimerHandle))
			GetWorldTimerManager().ClearTimer(LaserFireTimerHandle);
		else
			GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}
}

void ANR_Weapon::PreFire()
{
	OnWeaponFireStart.Broadcast(WeaponSetting.AnimWeaponInfo.CharacterFireAnimMontage);

	if (WeaponSetting.FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSetting.FireSound, GetActorLocation(), 0.3f);
	}

	FindEndLocation();
}

void ANR_Weapon::FireProjectile()
{
	FireTimer = 0.0f;

	uint8 NumberProjectile = WeaponSetting.NumberProjectileByShot;

	PreFire();

	if (ShootLocation)
	{
		FProjectileInfo ProjectileInfo;
		ProjectileInfo = GetProjectile();

		FVector SpawnLocation = ShootLocation->GetComponentLocation();
		FRotator SpawnRotation = ShootLocation->GetComponentRotation();

		float RotationToTarget = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, ShootEndLocation).Yaw;
		float RotationToTargetWithDispersion = RotationToTarget + UKismetMathLibrary::RandomFloatInRange(-WeaponSetting.DispersionWeapon, WeaponSetting.DispersionWeapon);

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
			SpawnRotation = FRotator(0.0f, RotationToTargetWithDispersion + WeaponSetting.DispersionWeapon * CoefConfusionShot, 0.0f);

			ANR_Projectile* Projectile = Cast<ANR_Projectile>(GetWorld()->SpawnActor(ProjectileInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawmParams));
			if (Projectile)
			{
				Projectile->InitProjectile(ProjectileInfo);
			}

			CoefConfusionShot = CoefConfusionShot + Confusion;
		}
	}
}

void ANR_Weapon::LaserFire()
{
	PreFire();

	FVector SpawnLocation = ShootLocation->GetComponentLocation();

	TArray<FHitResult> Hit;
	TArray<AActor*> Actors;


	UKismetSystemLibrary::LineTraceMulti(GetWorld(), SpawnLocation, (ShootEndLocation - SpawnLocation) * WeaponSetting.LaserDistance,
		ETraceTypeQuery::TraceTypeQuery3, false, Actors, EDrawDebugTrace::None, Hit, true, FLinearColor::Green, FLinearColor::Red, 5.0f);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponSetting.LaserFx, SpawnLocation, UKismetMathLibrary::FindLookAtRotation(SpawnLocation, ShootEndLocation));

	for (int i = 0; i < Hit.Num(); i++)
	{
		if (Hit[i].GetActor())
		{
			if (WeaponSetting.ProjectileSetting.HitDecal)
			{
				UMaterialInterface* Material = WeaponSetting.ProjectileSetting.HitDecal;

				if (Material && Hit[i].GetComponent())
				{
					UGameplayStatics::SpawnDecalAttached(Material, FVector(20.0f), Hit[i].GetComponent(), NAME_None, Hit[i].ImpactPoint,
						Hit[i].ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
				}
			}
			if (WeaponSetting.ProjectileSetting.HitFXs)
			{
				UParticleSystem* Particle = WeaponSetting.ProjectileSetting.HitFXs;

				if (Particle)
				{
					UGameplayStatics::SpawnEmitterAttached(Particle, Hit[i].GetComponent(), NAME_None, Hit[i].ImpactPoint, Hit[i].ImpactNormal.Rotation(),
						EAttachLocation::KeepWorldPosition);
				}
			}
			if (WeaponSetting.ProjectileSetting.HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSetting.ProjectileSetting.HitSound, Hit[i].ImpactPoint, 0.3f);
			}

			UGameplayStatics::ApplyPointDamage(Hit[i].GetActor(), WeaponSetting.LaserDamage, Hit[i].ImpactPoint, Hit[i], GetOwner()->GetInstigatorController(), this, nullptr);
		}
	}
}

void ANR_Weapon::FindEndLocation()
{
	if (IsPlayer)
	{
		FVector DecalLocation = FMath::LinePlaneIntersection(Pawn->MouseLocation, Pawn->LineEnd, ShootLocation->GetComponentLocation(), FVector(0, 0, 1));

		FVector tmpV = (Pawn->GetActorLocation() - DecalLocation);

		if (tmpV.Size() > SizeChanelToChangeShootDirectionLogic)
		{
			ShootEndLocation = DecalLocation;
		}
		else
		{
			ShootEndLocation = ShootLocation->GetComponentLocation() + ShootLocation->GetForwardVector() * 2000.0f;
		}
	}
	else
	{
		ShootEndLocation = Pawn->GetActorLocation();
	}
}
