// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_PlayerState.h"

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

TArray<int> ANR_PlayerState::GetAchievements()
{
	Achievements.Init(0, 9);
	return Achievements;
}

TArray<FVector2D> ANR_PlayerState::GetEarnedAchievements()
{
	TArray<int> Buff;
	TArray<FVector2D> EarnedAchievements;
	Buff.Init(0, 9);
	
	Buff[0] = (NumKilled >= 3) + (NumKilled >= 5) + (NumKilled >= 10);
	Buff[1] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);

	for (int i = 0; i < 9; i++)
	{
		if (Buff[i] > Achievements[i]) 
			EarnedAchievements.Add(FVector2D(i, Buff[i]));
	}

	//for testing. delete later!!
	int delta = 3 - EarnedAchievements.Num();
	if (delta)
	{
		for (int i = 0; i < delta; ++i)
		{
			EarnedAchievements.Add(FVector2D(0, 0));
		}
	}

	return EarnedAchievements;
}