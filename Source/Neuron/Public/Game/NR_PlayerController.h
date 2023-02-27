// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NR_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIncrementLife);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenMenuWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndGame, bool, bIsWin);

/**
 * 
 */
UCLASS()
class NEURON_API ANR_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	//Current life
	int32 PlayerLifes = 3;

public:

	//Inputs
	virtual void SetupInputComponent() override;

	//Delegate for change life
	FOnIncrementLife OnIncrementLife;
	//Delegate for respawn character
	FOnRespawn OnRespawn;
	//Delegate for open/close menu widget
	FOnOpenMenuWidget OnOpenMenuWidget;
	//Delegate what player was absolutely dead
	FOnEndGame OnEndGame;

	//Function for respawn
	void TryToRespawnPlayer();

	//Function for open menu widget
	void OpenCloseMenuWidget();
};
