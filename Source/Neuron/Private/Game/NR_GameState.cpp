// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/NR_GameState.h"
#include "Enemy/NR_EnemyCharacterBase.h"
#include "Enemy/Boss/NR_EnemyBoss.h"
#include "Game/NR_PlayerController.h"
#include "Kismet/GameplayStatics.h"

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

	OnBossPhaseStarts.Broadcast();
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
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANR_EnemyCharacterBase::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		Actor->Destroy();
	}

	OnWavePhaseEnds.Broadcast();

	GetWorldTimerManager().SetTimer(SpawnBoss, this, &ANR_GameState::StartBossPhase, 2.f, false, 1.f);

}

void ANR_GameState::DecrementEnemy()
{
	if (Enemies > 0) Enemies--;
}

void ANR_GameState::SetBoss(ANR_EnemyBoss* Enemy)
{
	Boss = Enemy;
}

void ANR_GameState::TimeIsOver()
{
	if (--TimeLeft <= 0 || !Boss)
	{
		GetWorldTimerManager().ClearTimer(BossTimer);
	}
}