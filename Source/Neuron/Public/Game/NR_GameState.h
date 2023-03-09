// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FuncLibrary/Type.h"
#include "NR_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharStatsChanged, FCharStats, NewStats);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWavePhaseEnds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossPhaseStarts);
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
		FOnWavePhaseEnds OnWavePhaseEnds;
	UPROPERTY(BlueprintAssignable)
		FOnBossPhaseStarts OnBossPhaseStarts;
	UPROPERTY(BlueprintAssignable)
		FOnBossDies OnBossDies;


protected:

	//Stuct with character stats
	FCharStats CharStats;

	//Number of enemies
	int Enemies = 0;
	bool NotSpawning = true;
	bool BossAlive = false;

	//Level number
	int CurrentLevel = 0;

public:

	//Function for applying changes to CharStats from inserted tokens
	UFUNCTION(BlueprintCallable)
		void ApplyChanges(TArray<int> Tokens);

	UFUNCTION(BlueprintCallable)
		void SetStats(FCharStats Stats) { CharStats = Stats; }

	void IncrementEnemies() { Enemies++; }

	void ToggleSpawningState() { NotSpawning = NotSpawning ? false : true; }

	void ToggleBossState() { BossAlive = BossAlive ? true : false; }

	UFUNCTION(BlueprintCallable)
		void StartBossPhase();

	UFUNCTION(BlueprintCallable)
		void BossKilled();

	void TryToChangePhase();
};
