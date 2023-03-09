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

	OnEndGame.Broadcast(false);
	OnOpenMenuWidget.Broadcast();
}

void ANR_PlayerController::OpenCloseMenuWidget()
{
	OnOpenMenuWidget.Broadcast();
}

void ANR_PlayerController::OpenCloseTokenWidget()
{
	OnOpenTokenWidget.Broadcast();
}

void ANR_PlayerController::OpenClosePickupWidget(FName ObjectName)
{
	OnOpenPickupWidget.Broadcast(ObjectName);
}