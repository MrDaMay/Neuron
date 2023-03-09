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

void ANR_PlayerState::CalculateEarnedAchievements()
{
	TArray<int> Buff;
	TArray<FVector2D> EarnedAchievements;
	Buff.Init(9, 0);
	
	Buff[0] = (NumKilled >= 3) + (NumKilled >= 5) + (NumKilled >= 10);
	Buff[1] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);

	for (int i = 0; i < 9; i++)
	{
		if (Buff[i] > Achievements[i]) 
			EarnedAchievements.Add(FVector2D(i, Buff[i]));
	}
	
}