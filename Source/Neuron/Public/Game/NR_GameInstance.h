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

	TArray<float> Achievements;
	TArray<int> Tokens;
	TArray<FName> Weapons;

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

	//Search function by name item
	UFUNCTION(BlueprintCallable)
		bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);
	UFUNCTION(BlueprintCallable)
		bool GetDropInfoByName(FName DropEnemy, FDropObjects& DropObject);



	//Update achievement at index by certain level
	UFUNCTION(BlueprintCallable)
		void UpdateAchievementAt(int idx, float LevelMultiplier);

	UFUNCTION(BlueprintCallable)
	void AddTokens(TArray<int> Buff);
	TArray<float> GetAchievements() { return Achievements; }
	TArray<int> GetTokens() { return Tokens; }

	void InitAchievements();

	void LoadSavedAchievements(TArray<int> Achievements);
	void InitTokens();

	UFUNCTION(BlueprintCallable)
	void SetName(FName Name) { PlayerName = Name; }
	UFUNCTION(BlueprintCallable)
	FName GetName() { return PlayerName; }


};
