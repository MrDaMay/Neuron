// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NR_PlayerController.h"
#include "Game/NR_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void ANR_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Key assignment in project settings
	InputComponent->BindAction("MenuWidget", EInputEvent::IE_Pressed, this, &ANR_PlayerController::OpenCloseMenuWidget).bExecuteWhenPaused = true;
}

void ANR_PlayerController::CharacterDead()
{

	Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(0))->LevelNumb = 1;

	OnEndGame.Broadcast(false);
	OnOpenAchieveWidget.Broadcast();
}

void ANR_PlayerController::OpenCloseMenuWidget()
{
	OnOpenMenuWidget.Broadcast();
}

void ANR_PlayerController::OpenCloseTokenWidget()
{
	 OnOpenTokenWidget.Broadcast();
}

void ANR_PlayerController::OpenCloseAchieveWidget()
{
	OnOpenAchieveWidget.Broadcast();
}