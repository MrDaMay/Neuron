// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/NR_Projectile.h"
#include "NR_ProjectileGrenade.generated.h"

/**
 * 
 */
UCLASS()
class NEURON_API ANR_ProjectileGrenade : public ANR_Projectile
{
	GENERATED_BODY()

private:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void ImpactProjectile() override;

	virtual void InitProjectile(FProjectileInfo InitParam) override;

	void Explose();

	//Timer start
	//Delay flag before explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		float TimeToExplose = 5.0f;
	FTimerHandle ExploseTimerHandle;
	//Timer end
};