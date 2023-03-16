// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/NR_EnemyCharacterBase.h"
#include "Enemy/Boss/NR_ProjectileWeaponBossComponent.h"
#include "Enemy/Boss/NR_LazerWeaponBossComponent.h"
#include "NR_EnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class NEURON_API ANR_EnemyBoss : public ANR_EnemyCharacterBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle ProjectileWeaponTimer;

	void FireProjectileWeapon();

public:
	ANR_EnemyBoss();

	//ProjectileWeaponComponent
	FORCEINLINE class UNR_ProjectileWeaponBossComponent* GetProjectileWeaponComponent() const { return ProjectileWeapon; }

	//LazerWeaponComponent
	FORCEINLINE class UNR_LazerWeaponBossComponent* GetLazerWeaponComponent() const { return LazerWeapon; }

	//Params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float TimeToProjectileFire = 1.0f;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UNR_ProjectileWeaponBossComponent* ProjectileWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = KillDCore, meta = (AllowPrivateAccess = "true"))
		class UNR_LazerWeaponBossComponent* LazerWeapon;
};
