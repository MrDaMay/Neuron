// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FuncLibrary/Type.h"
#include "NR_ProjectileWeaponBossComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEURON_API UNR_ProjectileWeaponBossComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNR_ProjectileWeaponBossComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		FProjectileInfo ProjectileInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* ProjectileSpawn;
};
