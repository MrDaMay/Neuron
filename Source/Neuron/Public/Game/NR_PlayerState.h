// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NR_PlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIncrementScore, float, Score);

/**
 * 
 */
UCLASS()
class NEURON_API ANR_PlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	//Score variable
	float PlayerScore = 0.0f;

public:

	//Delegate for widget
	UPROPERTY(BlueprintAssignable, Category = "IcrementScore")
		FOnIncrementScore OnIncrementScore;

	//Function for increment score
	void IncrementScore(float MutableScore);

	//Function for check win condition (if we have score mechanics)
	void CheckWinCondition(float CurrentScore);
};
