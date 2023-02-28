// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FuncLibrary/Type.h"
#include "NR_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEURON_API UNR_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	//Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetting")
		UDataTable* WeaponInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		UDataTable* EnemyInfoTable = nullptr;

	//Search function by name item
	UFUNCTION(BlueprintCallable)
		bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);
	UFUNCTION(BlueprintCallable)
		bool GetEnemyInfoByName(FName NameEnemy, FEnemyCharacters& Enemy);
};
