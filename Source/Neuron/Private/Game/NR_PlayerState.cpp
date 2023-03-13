// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetMathLibrary.h"
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
	TArray<FVector2D> BuffEarnedAchievements;

	Buff.Init(0, 9);
	
	Buff[0] = (NumKilled >= 3) + (NumKilled >= 5) + (NumKilled >= 10);
	Buff[1] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);
	Buff[2] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);
	Buff[3] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);
	Buff[4] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);
	Buff[5] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);
	Buff[6] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);
	Buff[7] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);
	Buff[8] = (NumBonus >= 3) + (NumBonus >= 5) + (NumBonus >= 10);

	for (int i = 0; i < 9; i++)
	{
		if (Buff[i] > Achievements[i]) 
			BuffEarnedAchievements.Add(FVector2D(i, Buff[i]));
	}

	BuffEarnedAchievements.Sort([](const FVector2D& A, const FVector2D& B) {
			return A.Y > B.Y;
	});

	if (!BuffEarnedAchievements.IsEmpty())
	{
		BuffEarnedAchievements.SetNum(3);
	}

	BuffEarnedAchievements.RemoveAll([](FVector2D& A) {
		return A == FVector2D(0, 0);
		});

	return BuffEarnedAchievements;
}