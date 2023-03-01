// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NR_ProjectileGrenade.h"
#include "Kismet/GameplayStatics.h"

void ANR_ProjectileGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void ANR_ProjectileGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANR_ProjectileGrenade::ImpactProjectile()
{
	GetWorldTimerManager().SetTimer(ExploseTimerHandle, this, &ANR_ProjectileGrenade::Explose, TimeToExplose, false, 0.0f);
}

void ANR_ProjectileGrenade::InitProjectile(FProjectileInfo InitParam)
{
	Super::InitProjectile(InitParam);
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

	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
		ProjectileSetting.ExploseMaxDamage, ProjectileSetting.ExploseMaxDamage * 0.2f,
		GetActorLocation(), ProjectileSetting.ProjectileInnerRadiusDamage, ProjectileSetting.ProjectileMaxRadiusDamage, 5, NULL, IgnoredActor, this, GetInstigatorController());

	//Debug start
	DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileInnerRadiusDamage, 14, FColor(181, 0, 0), true, -1, 0, 2);
	DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMaxRadiusDamage, 28, FColor(0, 0, 181), true, -1, 0, 2);
	//Debug end

	this->Destroy();
}
