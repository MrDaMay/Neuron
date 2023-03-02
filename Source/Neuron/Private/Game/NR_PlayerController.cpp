// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_PlayerController.h"

void ANR_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Key assignment in project settings
	InputComponent->BindAction("MenuWidget", EInputEvent::IE_Pressed, this, &ANR_PlayerController::OpenCloseMenuWidget).bExecuteWhenPaused = true;
}

void ANR_PlayerController::TryToRespawnPlayer()
{
	OnIncrementLife.Broadcast();

	//Increment life
	PlayerLifes--;

	//Сheck of life for the condition of defeat
	if (PlayerLifes <= 0)
	{
		OnEndGame.Broadcast(false);
		OnOpenMenuWidget.Broadcast();
	}
	else
		OnRespawn.Broadcast();
}

void ANR_PlayerController::OpenCloseMenuWidget()
{
	OnOpenMenuWidget.Broadcast();
}
