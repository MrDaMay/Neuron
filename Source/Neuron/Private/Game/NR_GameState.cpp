// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/NR_GameState.h"
#include "Enemy/NR_EnemyCharacterBase.h"
#include "Enemy/Boss/NR_EnemyBoss.h"
#include "Game/NR_PlayerController.h"
#include "Game/NR_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void ANR_GameState::ApplyChanges(TArray<int> Tokens)
{
	CharStats.CoefDamage += Tokens[0] * 0.02f;
	CharStats.CoefDamageResist += Tokens[1] * 0.1f;
	CharStats.CoefFireSpeed += Tokens[2] * 0.02f;
	CharStats.CoefMovementSpeed += Tokens[3] * 0.05f;

	if (Tokens[4])
	{
		CharStats.CoefDamage += Tokens[4] * 0.02f;
		CharStats.CoefDamageResist += Tokens[4] * 0.1f;
		CharStats.CoefFireSpeed += Tokens[4] * 0.02f;
		CharStats.CoefMovementSpeed += Tokens[4] * 0.05f;
	}

	OnCharStatsChanged.Broadcast(CharStats);
	OnTokensChanged.Broadcast(Tokens);
}

void ANR_GameState::StartBossPhase()
{
	GetWorldTimerManager().SetTimer(BossTimer, this, &ANR_GameState::TimeIsOver, 1.f, true, 1.f);
	TimeLeft = 7.f;

	OnBossPhaseStarts.Broadcast(Boss);
}

void ANR_GameState::BossKilled()
{
	OnBossDies.Broadcast();
	GetWorldTimerManager().SetTimer(StartNewLevel, this, &ANR_GameState::StartWavePhase, 5.f, false, 1.f);
}

void ANR_GameState::StartWavePhase()
{
	OnWavePhaseStarts.Broadcast();
	CurrentLevel++;
}

void ANR_GameState::EndWavePhase()
{
	CurrentCoutEnemiesForKill--;

	if (CurrentCoutEnemiesForKill <= 0)
		OnWavePhaseEnds.Broadcast();
}

void ANR_GameState::DecrementEnemy()
{
	if (Enemies > 0) Enemies--;
}

void ANR_GameState::StartSpawnEnemyTimer()
{
	CurrentCoutEnemy = { 0,0,0 };
	CurrentCoutEnemies = 0;

	for (int i = 0; i < EnemyCharacters.Num(); i++)
	{
		MaxSpawnEnemies += EnemyCharacters[i].CoutToSpawn;
	}

	CurrentCoutEnemiesForKill = MaxSpawnEnemies;

	GetWorldTimerManager().SetTimer(SpawnEnemyTimer,this, &ANR_GameState::ChoiseOfEnemyForSpawn, MaxTimeForSpawn / MaxSpawnEnemies, true, 0.0f);
}

void ANR_GameState::ChoiseOfEnemyForSpawn()
{
	if (CurrentCoutEnemies < MaxSpawnEnemies)
	{
		float RandomForSpawn = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);

		if (RandomForSpawn <= 0.7f && TrySpawnEnemy(0))
		{
		}
		else
		{
			if (RandomForSpawn <= 0.85f && TrySpawnEnemy(1))
			{
			}
			else
			{
				TrySpawnEnemy(2);
			}
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnEnemyTimer);
	}
}

bool ANR_GameState::TrySpawnEnemy(int i)
{
	if (CurrentCoutEnemy[i] < EnemyCharacters[i].CoutToSpawn)
	{
		CurrentCoutEnemy[i]++;

		int32 IndexSpawnBase = UKismetMathLibrary::RandomIntegerInRange(1, EnemySpawnBase.Num() - 1);
		EnemySpawnBase[IndexSpawnBase]->SpawnEnemy(EnemyCharacters[i].Enemy);

		return true;
	}
	else
	{
		return false;
	}

}


void ANR_GameState::TimeIsOver()
{
	if (--TimeLeft <= 0 || !Boss)
	{
		GetWorldTimerManager().ClearTimer(BossTimer);
	}
}