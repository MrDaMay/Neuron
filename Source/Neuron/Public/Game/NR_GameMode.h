// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NR_GameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndGameDelegate, bool, IsWin);

/**
 * 
 */
UCLASS()
class NEURON_API ANR_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	//Delegate for widget
	UPROPERTY(BlueprintAssignable, Category = "EndGame")
		FOnEndGameDelegate OnEndGameDelegate;

	//Event for create finish widget and end game
	void EndGame(bool IsWin);
};
