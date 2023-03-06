// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_GameInstance.h"

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
		UE_LOG(LogTemp, Warning, TEXT("USnowBall_TPSGameInstance::GetWeaponInfoByName - WeaponTable is NULL"))
	}

	return bIsFind;
}

bool UNR_GameInstance::GetEnemyInfoByName(FName NameEnemy, FEnemyCharacters& Enemy)
{
	bool bIsFind = false;

	if (EnemyInfoTable)
	{
		FEnemyCharacters* EnemyInfoRow = EnemyInfoTable->FindRow<FEnemyCharacters>(NameEnemy, "", false);

		if (EnemyInfoRow)
		{
			bIsFind = true;
			Enemy = *EnemyInfoRow;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USnowBall_TPSGameInstance::GetWeaponInfoByName - WeaponTable is NULL"))
	}

	return bIsFind;
}

void UNR_GameInstance::ApplyAchievements()
{
	InitialStats.CoefDamage *= (1 + Achievements[0] + Achievements[2] + Achievements[5] + Achievements[7]);

	InitialStats.CoefFireSpeed *= (1 + Achievements[3] + Achievements[5] + Achievements[7]);

	InitialStats.CoefDamageResist *=  (1 + Achievements[4] + Achievements[5] + Achievements[7]);

	InitialStats.CoefMovementSpeed *= (1 + Achievements[6] + Achievements[5] + Achievements[7]);
}

void UNR_GameInstance::UpdateAchievementAt(int idx, float LevelMultiplier)
{
	Achievements[idx] = LevelMultiplier;
}