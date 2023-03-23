// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_GameInstance.h"
#include "Kismet/GameplayStatics.h"

bool UNR_GameInstance::GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo)
{
	bool bIsFind = false;

	if (WeaponInfoTable)
	{
		FWeaponInfo* WeaponInfoRow = WeaponInfoTable->FindRow<FWeaponInfo>(NameWeapon, "", false);

		if (WeaponInfoRow)
		{
			bIsFind = true;
			OutInfo = *WeaponInfoRow;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UNR_GameInstance::GetWeaponInfoByName - WeaponTable is NULL"))
	}

	return bIsFind;
}

bool UNR_GameInstance::GetDropInfoByName(FName DropEnemy, FDropObjects& DropObject)
{
	bool bIsFind = false;

	if (DropInfoTable)
	{
		FDropObjects* DropInfoRow = DropInfoTable->FindRow<FDropObjects>(DropEnemy, "", false);

		if (DropInfoRow)
		{
			bIsFind = true;
			DropObject = *DropInfoRow;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UNR_GameInstance::GetDropInfoByName - WeaponTable is NULL"))
	}

	return bIsFind;
}

bool UNR_GameInstance::GetLevelSettingsInfoByName(FName Level, FEnemyLevelSettings& EnemyLevelSettings)
{
	bool bIsFind = false;

	if (LevelSettingsInfoTable)
	{
		FEnemyLevelSettings* LevelSettingsInfoRow = LevelSettingsInfoTable->FindRow<FEnemyLevelSettings>(Level, "", false);

		if (LevelSettingsInfoRow)
		{
			bIsFind = true;
			EnemyLevelSettings = *LevelSettingsInfoRow;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UNR_GameInstance::GetLevelSettingsInfoByName - WeaponTable is NULL"))
	}

	return bIsFind;
}


void UNR_GameInstance::UpdateAchievementAt(int idx, float LevelMultiplier)
{
	Achievements[idx] = LevelMultiplier;
}

void UNR_GameInstance::AddTokens(TArray<int> Buff)
{
	for (int i = 0; i < 6; i++)
	{
		Tokens[i] += Buff[i];
	}
}

void UNR_GameInstance::InitAchievements()
{
	for (int i = 0; i < 9; i++)
	{
		Achievements.Add(0);
	}
}

void UNR_GameInstance::LoadSavedAchievements(TArray<int> Buff)
{

	for (int i = 0; i < 9; i++)
	{
		Achievements.Add(Buff[i]);
	}
}

void UNR_GameInstance::InitTokens()
{
	for (int i = 0; i < 6; i++)
	{
		Tokens.Add(0);
	}
}

void UNR_GameInstance::SaveGame()
{
	SaveGameSlot->Weapons = Weapons;
	SaveGameSlot->Achievements = Achievements;
	SaveGameSlot->Tokens = Tokens;

	UGameplayStatics::SaveGameToSlot(SaveGameSlot, "SaveGame", 0);
}

void UNR_GameInstance::LoadGame()
{
	if(UGameplayStatics::DoesSaveGameExist("SaveGame", 0))
	{
		SaveGameSlot = Cast<UNR_SaveGame>(UGameplayStatics::LoadGameFromSlot("SaveGame", 0));

		Weapons = SaveGameSlot->Weapons;
		Achievements = SaveGameSlot->Achievements;
		Tokens = SaveGameSlot->Tokens;
	}
}
