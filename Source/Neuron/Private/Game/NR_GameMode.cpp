// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_GameMode.h"

void ANR_GameMode::BeginPlay()
{
	Super::BeginPlay();

}

void ANR_GameMode::EndGame(bool IsWin)
{
	OnEndGameDelegate.Broadcast(IsWin);
}

void ANR_GameMode::SpawnEnemies()
{
	OnSpawnEnemy.Broadcast();
}

