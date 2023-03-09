// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/NR_EnemyBoss.h"

void ANR_EnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ProjectileWeaponTimer, this, &ANR_EnemyBoss::FireProjectileWeapon, 1.0f, true, 1.0f);
}

void ANR_EnemyBoss::FireProjectileWeapon()
{
	ProjectileWeapon->SpawnProjectile();
}

ANR_EnemyBoss::ANR_EnemyBoss()
{
	ProjectileWeapon = CreateDefaultSubobject<UNR_ProjectileWeaponBossComponent>(TEXT("ProjectileWeapon"));

	LazerWeapon = CreateDefaultSubobject<UNR_LazerWeaponBossComponent>(TEXT("LazerWeapon"));
}
