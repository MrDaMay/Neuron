// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_PlayerState.h"

void ANR_PlayerState::IncrementScore(float MutableScore)
{
	PlayerScore += MutableScore;

	OnIncrementScore.Broadcast(PlayerScore);
}

void ANR_PlayerState::CheckWinCondition(float CurrentScore)
{
	//ToDo add code and delegate
}
