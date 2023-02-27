// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_GameMode.h"

void ANR_GameMode::EndGame(bool bIsWin)
{
	OnEndGame.Broadcast(bIsWin);
}
