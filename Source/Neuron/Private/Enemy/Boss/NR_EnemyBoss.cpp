// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/NR_EnemyBoss.h"
#include "Collectable/NR_CollectableBase.h"
#include "Game/NR_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

void ANR_EnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ProjectileWeaponTimer, this, &ANR_EnemyBoss::FireProjectileWeapon, TimeToProjectileFire, true, TimeToProjectileFire);

	if (SpawnAnimation)
	{
		PlayAnimMontage(SpawnAnimation);
	}
}

void ANR_EnemyBoss::FireProjectileWeapon()
{
	ProjectileWeapon->SpawnProjectile();
}

void ANR_EnemyBoss::FindWeaponForSpawn()
{
	auto GameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance->LevelNumb - 1 == 1)
	{
		SpawnWeaponName = "Gun";
	}
	else
	{
		if (GameInstance->LevelNumb - 1 == 2)
		{
			SpawnWeaponName = "Lazer";
		}
		else
		{
			if (GameInstance->LevelNumb - 1 == 3)
			{
				SpawnWeaponName = "GrenadeLauncher";
			}
		}
	}
}


ANR_EnemyBoss::ANR_EnemyBoss()
{
	ProjectileWeapon = CreateDefaultSubobject<UNR_ProjectileWeaponBossComponent>(TEXT("ProjectileWeapon"));

	LazerWeapon = CreateDefaultSubobject<UNR_LazerWeaponBossComponent>(TEXT("LazerWeapon"));
}

void ANR_EnemyBoss::Drop()
{
	FindWeaponForSpawn();

	if (SpawnWeaponName != "None")
	{
		if (DropChance >= UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f))
		{
			FDropObjects SpawnObject;

			FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 94);
			FRotator SpawnRotator = FRotator(0);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			auto GameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			GameInstance->GetDropInfoByName(SpawnWeaponName, SpawnObject);

			auto SpawnActor = Cast<ANR_CollectableBase>(GetWorld()->SpawnActor(SpawnObject.DropItem, &SpawnLocation, &SpawnRotator, SpawnParams));
			if (SpawnActor)
			{
				SpawnActor->StaticMesh->SetStaticMesh(SpawnObject.DropMesh);
				SpawnActor->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
				SpawnActor->LightPartical->Template = SpawnObject.LightPartical;
				SpawnActor->OverlapSpawnPartical = SpawnObject.OverlapPartical;
				SpawnActor->ObjectName = SpawnWeaponName;
				SpawnActor->PickUpSound = SpawnObject.PickUpSound;
			}
		}
	}
}
