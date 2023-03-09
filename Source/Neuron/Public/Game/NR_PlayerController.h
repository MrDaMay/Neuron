// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NR_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIncrementLife);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenMenuWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenTokenWidget); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenAchieveWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenPickupWidget, FName, ObjectName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndGame, bool, bIsWin);

/**
 * 
 */
UCLASS()
class NEURON_API ANR_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	//For Token widget
	bool bTokensAvailable = false;

public:

	//Inputs
	virtual void SetupInputComponent() override;

	//Delegate for change life
	FOnIncrementLife OnIncrementLife;
	//Delegate for respawn character
	FOnRespawn OnRespawn;
	//Delegate for open/close menu widget
	UPROPERTY(BlueprintAssignable, Category = "Widgets")
		FOnOpenMenuWidget OnOpenMenuWidget;
	//Delegate for open/close pick up widget
	UPROPERTY(BlueprintAssignable, Category = "Widgets")
		FOnOpenPickupWidget OnOpenPickupWidget;
	//Delegate for open/close token widget
	UPROPERTY(BlueprintAssignable, Category = "Widgets")
		FOnOpenTokenWidget OnOpenTokenWidget;
	UPROPERTY(BlueprintAssignable, Category = "Widgets")
		FOnOpenAchieveWidget OnOpenAchieveWidget;
	//Delegate what player was absolutely dead
	FOnEndGame OnEndGame;

	//Function for respawn
	void TryToRespawnPlayer();

	//Function for open menu widget
	void OpenCloseMenuWidget();

	//Function for open menu widget
	void OpenCloseTokenWidget();

	//Function for open menu widget
	void OpenCloseAchieveWidget();

	void BlockTokenWidget() { bTokensAvailable = false; }
	void UnblockTokenWidget() { bTokensAvailable = true; }

	//Fuction for open/close pickup widget
	void OpenClosePickupWidget(FName ObjectName);
};
