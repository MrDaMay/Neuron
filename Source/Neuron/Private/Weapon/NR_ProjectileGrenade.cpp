// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NR_ProjectileGrenade.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

void ANR_ProjectileGrenade::BeginPlay()
{
	Super::BeginPlay();

	BulletCollisionSphere->SetEnableGravity(true);
}

void ANR_ProjectileGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANR_ProjectileGrenade::ImpactProjectile()
{
	GetWorldTimerManager().SetTimer(ExploseTimerHandle, this, &ANR_ProjectileGrenade::Explose, ProjectileSetting.TimeToExplose, false, ProjectileSetting.TimeToExplose);
}

void ANR_ProjectileGrenade::InitProjectile(FProjectileInfo InitParam, float CoefDamage)
{
	Super::InitProjectile(InitParam, CoefDamage);
}

void ANR_ProjectileGrenade::Explose()
{
	if (ProjectileSetting.ExploseFX)
	{
		FVector VectorToExplose = FVector(0.0f, 0.0f, 0.0f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExploseFX, GetActorLocation(),
			VectorToExplose.Rotation());
	}
	if (ProjectileSetting.ExploseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.ExploseSound, GetActorLocation(), 0.3f);
	}

	TArray<AActor*> IgnoredActor;

	UGameplayStatics::ApplyRadialDamage(GetWorld(),
		ProjectileSetting.ExploseMaxDamage,
		GetActorLocation(), ProjectileSetting.ProjectileMaxRadiusDamage, NULL, IgnoredActor, this, GetInstigatorController());

	//Debug start
	DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMaxRadiusDamage, 14, FColor(181, 0, 0), true, -1, 0, 2);
	//Debug end

	this->Destroy();
}
