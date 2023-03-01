// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_GameMode.h"

void ANR_GameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SpawnEnemyTimer, this, &ANR_GameMode::SpawnEnemys, 1.0f, true, 1.0f);
}

void ANR_GameMode::EndGame(bool IsWin)
{
	OnEndGameDelegate.Broadcast(IsWin);
}

void ANR_GameMode::SpawnEnemys()
{
	OnSpawnEnemy.Broadcast();
}
