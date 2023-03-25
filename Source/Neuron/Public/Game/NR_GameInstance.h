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

	//Achviements Array, contains achievement index and its level for stats increasing

	TArray<float> Achievements = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	TArray<int> Tokens = {0, 0, 0, 0, 0, 0};
	TArray<float> Counters;

	FName PlayerName;

public:
	//Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetting")
		UDataTable* WeaponInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
		UDataTable* DropInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievements")
		UDataTable* AchievementsInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TokensMultiplyers")
		UDataTable* TokensInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelSettings")
		UDataTable* LevelSettingsInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		UDataTable* LevelsTable = nullptr;

	//Search function by name item
	UFUNCTION(BlueprintCallable)
		bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);
	UFUNCTION(BlueprintCallable)
		bool GetDropInfoByName(FName DropEnemy, FDropObjects& DropObject);

	UFUNCTION(BlueprintCallable)
		bool GetLevelSettingsInfoByName(FName Level, FEnemyLevelSettings& EnemyLevelSettings);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
		int32 LevelNumb = 1;


	//Update achievement at index by certain level
	UFUNCTION(BlueprintCallable)
		void UpdateAchievementAt(int idx, float LevelMultiplier);

	UFUNCTION(BlueprintCallable)
	void AddTokens(TArray<int> Buff);
	UFUNCTION(BlueprintCallable)
	TArray<float> GetAchievements() { return Achievements; }
	UFUNCTION(BlueprintCallable)
	TArray<int> GetTokens() { return Tokens; }

	void InitAchievements();

	void LoadSavedAchievements(TArray<int> Achievements);
	void InitTokens();

	void SetCounters(TArray<float> Buffer) { Counters = Buffer; };
	TArray<float> GetCounters() { return Counters; }
	void ClearCounters() { Counters.Empty(); };

	UFUNCTION(BlueprintCallable)
	void SetName(FName Name) { PlayerName = Name; }
	UFUNCTION(BlueprintCallable)
	FName GetName() { return PlayerName; }

	//Weapon slot in character
	TArray<FName> Weapons{ "Rifle" };

	//Reset for a new loop
	UFUNCTION(BlueprintCallable)
	void ResetInstance();

	//Save game
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();
};
