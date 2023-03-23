// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FuncLibrary/Type.h"
#include "Enemy/NR_EnemySpawnBase.h"
#include "NR_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharStatsChanged, FCharStats, NewStats);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTokensChanged, TArray<int>, Tokens);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWavePhaseStarts);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWavePhaseEnds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeMap);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartChangeLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossPhaseStarts, TSubclassOf<class ANR_EnemyBoss>, Boss);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDies);
/**
 * 
 */
UCLASS()
class NEURON_API ANR_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	//Delegate for stats changing
	UPROPERTY(BlueprintAssignable)
		FOnCharStatsChanged OnCharStatsChanged;
	UPROPERTY(BlueprintAssignable)
		FOnTokensChanged OnTokensChanged;
	UPROPERTY(BlueprintAssignable)
		FOnWavePhaseEnds OnWavePhaseStarts;
	UPROPERTY(BlueprintAssignable)
		FOnWavePhaseEnds OnWavePhaseEnds;
	UPROPERTY(BlueprintAssignable)
		FOnBossPhaseStarts OnBossPhaseStarts;
	UPROPERTY(BlueprintAssignable)
		FOnBossDies OnBossDies;

	FOnChangeMap OnChangeMap;
	FOnStartChangeLevel OnStartChangeLevel;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Timer for measure how fast player will kill the boss
	FTimerHandle BossTimer;
	FTimerHandle SpawnBoss;
	float TimeLeft = 0.f;

	//Stuct with character stats
	FCharStats CharStats;

	//Number of enemies
	int Enemies = 0;
	bool NotSpawning = true;
	bool BossAlive = false;

	//Level number
	FTimerHandle StartNewLevel;

	//Spawn start
	TArray<int32> CurrentCoutEnemy;
	int32 MaxSpawnEnemies;
	int32 CurrentCoutEnemies;
	FTimerHandle SpawnEnemyTimer;
	void StartSpawnEnemyTimer();
	void ChoiseOfEnemyForSpawn();
	bool TrySpawnEnemy(int i);
	//Spawn end

public:

	//Function for applying changes to CharStats from inserted tokens
	UFUNCTION(BlueprintCallable)
		void ApplyChanges(TArray<int> Tokens);

	UFUNCTION(BlueprintCallable)
		void SetStats(FCharStats Stats) { CharStats = Stats; }

	void IncrementEnemies() { Enemies++; }

	void ToggleSpawningState() { NotSpawning = NotSpawning ? false : true; }

	void ToggleBossState() { BossAlive = BossAlive ? false : true; }

	void TimeIsOver();

	UFUNCTION(BlueprintCallable)
		void StartBossPhase();
	UFUNCTION(BlueprintCallable)
		void BossKilled();
	UFUNCTION(BlueprintCallable)
		void StartWavePhase();
	UFUNCTION(BlueprintCallable)
		void DecreaseEnemies();
	UFUNCTION(BlueprintCallable)
	void ChangeLevel();

	//Spawn start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	int32 CurrentCoutEnemiesForKill;
	TArray<ANR_EnemySpawnBase*> EnemySpawnBase;
	FEnemyLevelSettings LevelSettingForSpawn;
	void TrySpawnBoss();
	//Spawn end
};
