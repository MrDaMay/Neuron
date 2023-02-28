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
