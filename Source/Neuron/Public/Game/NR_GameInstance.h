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

protected:

	//Initial Character Stats, based on achivements
	FCharStats InitialStats;
	//Achviements Array, contains achievement index and its level for stats increasing
	TArray<float> Achievements;

public:
	//Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetting")
		UDataTable* WeaponInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		UDataTable* EnemyInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
		UDataTable* DropInfoTable = nullptr;

	//Search function by name item
	UFUNCTION(BlueprintCallable)
		bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);
	UFUNCTION(BlueprintCallable)
		bool GetDropInfoByName(FName DropEnemy, FDropObjects& DropObject);


	//Apply achievements on CharacterStats
	UFUNCTION(BlueprintCallable)
		void ApplyAchievements();

	//Update achievement at index by certain level
	UFUNCTION(BlueprintCallable)
		void UpdateAchievementAt(int idx, float LevelMultiplier);

	//Update Achievement at index by certain level
	UFUNCTION(BlueprintCallable)
		FCharStats GetInitialStats() { return InitialStats; }


};
