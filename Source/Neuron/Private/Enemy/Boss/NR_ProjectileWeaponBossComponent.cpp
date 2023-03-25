// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/NR_ProjectileWeaponBossComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/NR_Projectile.h"


// Sets default values for this component's properties
UNR_ProjectileWeaponBossComponent::UNR_ProjectileWeaponBossComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called every frame
void UNR_ProjectileWeaponBossComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNR_ProjectileWeaponBossComponent::SpawnProjectile()
{
	uint8 NumberProjectile = 12;

	FVector SpawnLocation = GetOwner()->GetActorLocation();
	FRotator SpawnRotation = FRotator(0);

	FActorSpawnParameters SpawmParams;
	SpawmParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawmParams.Owner = GetOwner();


	int Shift = UKismetMathLibrary::RandomIntegerInRange(0, 4);
	float CoefConfusionShot = 0.0f + 7.5 * Shift;

	if(ProjectileSpawn)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSpawn, GetOwner()->GetActorLocation());

	for (int8 i = 0; i < NumberProjectile; i++)
	{

		SpawnRotation = FRotator(0.0f,  CoefConfusionShot, 0.0f);

		ANR_Projectile* Projectile = Cast<ANR_Projectile>(GetWorld()->SpawnActor(ProjectileInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawmParams));
		if(Projectile)
			Projectile->InitProjectile(ProjectileInfo, 1.0f);

		CoefConfusionShot = CoefConfusionShot + 30.0f;
	}
}

