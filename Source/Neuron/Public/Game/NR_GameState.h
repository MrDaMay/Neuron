// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FuncLibrary/Type.h"
#include "NR_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharStatsChanged, FCharStats, NewStats);
/**
 * 
 */
UCLASS()
class NEURON_API ANR_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	//Delegate for stats changing
	UPROPERTY(BlueprintAssignable)
		FOnCharStatsChanged OnCharStatsChanged;


protected:

	//Stuct with character stats
	FCharStats CharStats;

public:

	//Function for applying changes to CharStats from inserted tokens
	UFUNCTION(BlueprintCallable)
		void ApplyChanges(TArray<int> Tokens);

	UFUNCTION(BlueprintCallable)
		void SetStats(FCharStats Stats) { CharStats = Stats; }

	
};
