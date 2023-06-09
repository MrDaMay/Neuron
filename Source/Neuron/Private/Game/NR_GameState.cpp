// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/NR_GameState.h"
#include "Enemy/Boss/NR_EnemyBoss.h"
#include "Game/NR_GameInstance.h"
#include "Game/NR_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Collectable/NR_CollectableBase.h"


void ANR_GameState::BeginPlay()
{
	Super::BeginPlay();

	auto myGameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (myGameInstance)
	{
		if(!myGameInstance->GetLevelSettingsInfoByName(*FString::FromInt(myGameInstance->LevelNumb), LevelSettingForSpawn))
		{
			myGameInstance->GetLevelSettingsInfoByName(*FString::FromInt(myGameInstance->LevelSettingsInfoTable->GetRowNames().Num()), LevelSettingForSpawn);
			myGameInstance->LoadGame();
		}
	}

	StartWavePhase();
}

void ANR_GameState::ApplyChanges(TArray<int> Tokens)
{
	auto MyGameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstance->AddTokens(Tokens);

	OnTokensChanged.Broadcast(Tokens);

	int count = 0;
	for (int i = 0; i < 6; ++i)
	{
		count += Tokens[i];
	}

	Cast<ANR_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0))->AddTokens(count);
}

void ANR_GameState::StartBossPhase()
{

	ClearLevel();

	GetWorldTimerManager().SetTimer(SpawnBoss, this, &ANR_GameState::TrySpawnBoss, 1.f, true, 0.f);
	OnChangeMap.Broadcast();
	OnBossPhaseStarts.Broadcast(LevelSettingForSpawn.Boss);
	
}

void ANR_GameState::BossKilled()
{
	OnBossDies.Broadcast();

	auto myPlayerState = Cast<ANR_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (myPlayerState)
	{
		myPlayerState->SetBossTime(TimeLeft);
	}

	auto myGameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (myGameInstance)
		myGameInstance->LevelNumb++;
}

void ANR_GameState::StartWavePhase()
{
	OnWavePhaseStarts.Broadcast();

	StartSpawnEnemyTimer();
}

void ANR_GameState::DecreaseEnemies()
{
	CurrentCoutEnemiesForKill--;

	if (CurrentCoutEnemiesForKill == 0)
		OnWavePhaseEnds.Broadcast();
}

void ANR_GameState::ChangeLevel()
{
	OnStartChangeLevel.Broadcast();

	// Change this function
	auto LevelTable = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->LevelsTable;
	TArray<FName> LevelNames = LevelTable->GetRowNames();

	auto myPlayerState = Cast<ANR_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (myPlayerState)
	{
		myPlayerState->IncrementLevel();
		myPlayerState->SaveCounters();
	}

	int idx = UKismetMathLibrary::RandomIntegerInRange(0, LevelNames.Num() - 2);

	UGameplayStatics::OpenLevel(GetWorld(), LevelNames[idx]);
}

void ANR_GameState::ClearLevel()
{
	TArray<AActor*> DroppedObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANR_CollectableBase::StaticClass(), DroppedObjects);

	for (auto Object : DroppedObjects)
	{
		Object->Destroy();
	}
}

void ANR_GameState::StartSpawnEnemyTimer()
{
	if (EnemySpawnBase.Num() > 0 && !GetWorldTimerManager().IsTimerActive(SpawnEnemyTimer))
	{
		CurrentCoutEnemy = { 0,0,0 };
		CurrentCoutEnemies = 0;

		for (int i = 0; i < LevelSettingForSpawn.EnemyCharacters.Num(); i++)
		{
			MaxSpawnEnemies += LevelSettingForSpawn.EnemyCharacters[i].CoutToSpawn;
		}

		CurrentCoutEnemiesForKill = MaxSpawnEnemies;

		GetWorldTimerManager().SetTimer(SpawnEnemyTimer, this, &ANR_GameState::ChoiseOfEnemyForSpawn, LevelSettingForSpawn.MaxTimeForSpawn / MaxSpawnEnemies, true, 0.0f);
	}
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
		if (!EnemySpawnBase.IsEmpty())
		{
			int32 IndexSpawnBase = UKismetMathLibrary::RandomIntegerInRange(1, EnemySpawnBase.Num() - 1);
			EnemySpawnBase[IndexSpawnBase]->SpawnBoss(LevelSettingForSpawn.Boss);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnBoss);

		TimeLeft = 0.f;
		GetWorldTimerManager().SetTimer(BossTimer, this, &ANR_GameState::TimeIsOver, 1.f, true, 1.f);
	}
}


void ANR_GameState::TimeIsOver()
{
	if (++TimeLeft > 12 || !LevelSettingForSpawn.Boss)
	{
		GetWorldTimerManager().ClearTimer(BossTimer);
	}
}
