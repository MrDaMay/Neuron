// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NR_GameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndGameDelegate, bool, IsWin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnEnemy);

/**
 * 
 */
UCLASS()
class NEURON_API ANR_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	//Spawn timer
	FTimerHandle SpawnEnemyTimer;

	//Delegate for widget
	UPROPERTY(BlueprintAssignable, Category = "EndGame")
		FOnEndGameDelegate OnEndGameDelegate;

	FOnSpawnEnemy OnSpawnEnemy;

	//Event for create finish widget and end game
	void EndGame(bool IsWin);

	void SpawnEnemies();
};