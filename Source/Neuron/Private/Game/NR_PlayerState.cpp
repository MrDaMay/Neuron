// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/NR_PlayerState.h"
#include "Game/NR_GameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void ANR_PlayerState::IncrementScore(float MutableScore)
{
	PlayerScore += MutableScore;

	NumKilled++;

	OnIncrementScore.Broadcast(PlayerScore);
}

void ANR_PlayerState::IncrementNumBonus()
{
	NumBonus++;
}

void ANR_PlayerState::CheckWinCondition(float CurrentScore)
{
	//ToDo add code and delegate
}

void ANR_PlayerState::SetNoDamageLevel()
{
	NoDamageLevels = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->LevelNumb - 1;
}

void ANR_PlayerState::SetBossTime(int Seconds)
{
	BossTime = Seconds;
}

void ANR_PlayerState::AddTokens(int Num)
{
	Tokens += Num;
}

void ANR_PlayerState::SaveCounters()
{
	TArray<float> Buffer;

	Buffer.Add(PlayerScore);
	Buffer.Add(NumKilled);
	Buffer.Add(NumBonus);
	Buffer.Add(BossTime);
	Buffer.Add(SurviveTime);
	Buffer.Add(NoDamageLevels);
	Buffer.Add(PassedLevels);
	Buffer.Add(NumBossKilled);
	Buffer.Add(CurrentLevel);
	Buffer.Add(Tokens);

	Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetCounters(Buffer);

}

void ANR_PlayerState::LoadCounters()
{
	TArray<float> Buff = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetCounters();

	if (!Buff.IsEmpty() && Buff.Num() == 10)
	{
		PlayerScore = Buff[0];
		NumKilled = Buff[1];
		NumBonus = Buff[2];
		BossTime = Buff[3];
		SurviveTime = Buff[4];
		NoDamageLevels = Buff[5];
		PassedLevels = Buff[6];
		NumBossKilled = Buff[7];
		CurrentLevel = Buff[8];
		Tokens = Buff[9];
	}
}


TArray<int> ANR_PlayerState::GetAchievements()
{
	Achievements.Init(0, 9);
	return Achievements;
}

TArray<FVector2D> ANR_PlayerState::GetEarnedAchievements()
{
	TArray<int> Buff;
	TArray<FVector2D> BuffEarnedAchievements;
	TArray<int> Conditions;

	auto GameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto Table = GameInstance->AchievementsInfoTable;
	
	TArray<FName> RowNames = Table->GetRowNames();

	for (auto Name : RowNames)
	{
		auto Row = Table->FindRow<FAchivementsInfo>(Name, "");
		auto Levels = Row->Levels;

		for (auto Level : Levels)
		{
			Conditions.Add(Level.Requirement);
		}
	}


	Buff.Init(0, 9);
	
	Buff[0] = (NumKilled >= Conditions[0]) + (NumKilled >= Conditions[1]) + (NumKilled >= Conditions[2]);
	Buff[1] = (NumBonus >= Conditions[4]) + (NumBonus >= Conditions[5]) + (NumBonus >= Conditions[6]);
	Buff[2] = 0;
	Buff[3] = (BossTime <= Conditions[11]) + (BossTime <= Conditions[12]) + (BossTime <= Conditions[13]);
	Buff[4] = (SurviveTime >= Conditions[15]) + (SurviveTime >= Conditions[16]) + (SurviveTime >= Conditions[17]);
	Buff[5] = (NoDamageLevels >= Conditions[19]) + (NoDamageLevels >= Conditions[20]) + (NoDamageLevels >= Conditions[21]);
	Buff[6] = ((PassedLevels + 1) >= Conditions[23]) + ((PassedLevels + 1) >= Conditions[24]) + ((PassedLevels + 1) >= Conditions[25]);
	Buff[7] = (Tokens >= Conditions[27]) + (Tokens >= Conditions[28]) + (Tokens >= Conditions[29]);
	Buff[8] = 1;

	for (int i = 0; i < 8; i++)
	{
		if (Buff[i] == 0)
		{
			Buff[8] = 0;
		}
	}

	for (int i = 0; i < 9; i++)
	{
		if (Buff[i] > Achievements[i]) 
			BuffEarnedAchievements.Add(FVector2D(i, Buff[i]));
	}

	BuffEarnedAchievements.Sort([](const FVector2D& A, const FVector2D& B) {
			return A.Y > B.Y;
	});

	if (BuffEarnedAchievements.Num() > 3)
	{
		BuffEarnedAchievements.SetNum(3);
	}

	BuffEarnedAchievements.RemoveAll([](FVector2D& A) {
		return A == FVector2D(0, 0);
		});

	return BuffEarnedAchievements;
}