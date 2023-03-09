// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/NR_EnemyCharacterBase.h"
#include "NR_EnemyCharacterShooter.generated.h"

/**
 * 
 */
UCLASS()
class NEURON_API ANR_EnemyCharacterShooter : public ANR_EnemyCharacterBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	//Spawn weapon
	void SpawnWeapon();

	//Attack override
	virtual	void AttackEnemy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload) override;

	//Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		ANR_Weapon* CurrentWeapon = nullptr;
};
