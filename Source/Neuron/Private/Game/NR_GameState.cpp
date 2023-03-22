// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/NR_GameState.h"
#include "Enemy/Boss/NR_EnemyBoss.h"
#include "Game/NR_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void ANR_GameState::BeginPlay()
{
	Super::BeginPlay();

	auto myGameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (myGameInstance)
	{
		if(!myGameInstance->GetLevelSettingsInfoByName(*FString::FromInt(myGameInstance->LevelNumb), LevelSettingForSpawn))
		{
			myGameInstance->GetLevelSettingsInfoByName(*FString::FromInt(myGameInstance->LevelSettingsInfoTable->GetRowNames().Num()), LevelSettingForSpawn);
		}
	}

	StartSpawnEnemyTimer();
}

void ANR_GameState::ApplyChanges(TArray<int> Tokens)
{
	auto MyGameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstance->AddTokens(Tokens);

	OnTokensChanged.Broadcast(Tokens);
}

void ANR_GameState::StartBossPhase()
{

	GetWorldTimerManager().SetTimer(SpawnBoss, this, &ANR_GameState::TrySpawnBoss, 1.f, true, 0.f);
	OnChangeMap.Broadcast();
	OnBossPhaseStarts.Broadcast(Boss);
	
}

void ANR_GameState::BossKilled()
{
	OnBossDies.Broadcast();

	auto myGameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (myGameInstance)
		myGameInstance->LevelNumb++;

	GetWorldTimerManager().SetTimer(StartNewLevel, this, &ANR_GameState::ChangeLevel, 20.0f, false, 20.0f);
}

void ANR_GameState::StartWavePhase()
{
	OnWavePhaseStarts.Broadcast();
}

void ANR_GameState::DecreaseEnemies()
{
	CurrentCoutEnemiesForKill--;

	if (CurrentCoutEnemiesForKill <= 0)
		OnWavePhaseEnds.Broadcast();
}

void ANR_GameState::ChangeLevel()
{
	// Change this function
	auto LevelTable = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->LevelsTable;
	TArray<FName> LevelNames = LevelTable->GetRowNames();

	int idx = UKismetMathLibrary::RandomIntegerInRange(0, LevelNames.Num() - 1);

	UGameplayStatics::OpenLevel(GetWorld(), LevelNames[idx]);
}

void ANR_GameState::StartSpawnEnemyTimer()
{
	CurrentCoutEnemy = { 0,0,0 };
	CurrentCoutEnemies = 0;

	for (int i = 0; i < LevelSettingForSpawn.EnemyCharacters.Num(); i++)
	{
		MaxSpawnEnemies += LevelSettingForSpawn.EnemyCharacters[i].CoutToSpawn;
	}

	CurrentCoutEnemiesForKill = MaxSpawnEnemies;

	GetWorldTimerManager().SetTimer(SpawnEnemyTimer,this, &ANR_GameState::ChoiseOfEnemyForSpawn, LevelSettingForSpawn.MaxTimeForSpawn / MaxSpawnEnemies, true, 0.0f);
}

void ANR_GameState::ChoiseOfEnemyForSpawn()
{
	if (CurrentCoutEnemies <= MaxSpawnEnemies)
	{
		float RandomForSpawn = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);

		if (RandomForSpawn <= 0.7f && TrySpawnEnemy(0) || LevelSettingForSpawn.EnemyCharacters.Num() == 1)
		{
			TrySpawnEnemy(0);
		}
		else
		{
			if (RandomForSpawn <= 0.85f && TrySpawnEnemy(1) || LevelSettingForSpawn.EnemyCharacters.Num() == 2)
			{
				TrySpawnEnemy(1);
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
		OnWavePhaseEnds.Broadcast();
	}
}

bool ANR_GameState::TrySpawnEnemy(int i)
{
	if (CurrentCoutEnemy[i] < LevelSettingForSpawn.EnemyCharacters[i].CoutToSpawn)
	{
		CurrentCoutEnemy[i]++;

		int32 IndexSpawnBase = UKismetMathLibrary::RandomIntegerInRange(0, EnemySpawnBase.Num() - 1);
		EnemySpawnBase[IndexSpawnBase]->SpawnEnemy(LevelSettingForSpawn.EnemyCharacters[i].Enemy);

		return true;
	}
	else
	{
		return false;
	}

}

void ANR_GameState::TrySpawnBoss()
{
	if (!BossAlive)
	{
		int32 IndexSpawnBase = UKismetMathLibrary::RandomIntegerInRange(1, EnemySpawnBase.Num() - 1);
		EnemySpawnBase[IndexSpawnBase]->SpawnBoss(Boss);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnBoss);

		TimeLeft = 7.f;
		GetWorldTimerManager().SetTimer(BossTimer, this, &ANR_GameState::TimeIsOver, 1.f, true, 1.f);
	}
}


void ANR_GameState::TimeIsOver()
{
	if (--TimeLeft <= 0 || !Boss)
	{
		GetWorldTimerManager().ClearTimer(BossTimer);
	}
}
