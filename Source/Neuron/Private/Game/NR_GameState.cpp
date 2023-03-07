// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_GameState.h"

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
}
