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
	// 
	float NumKilled = 0.0f;
	//
	float NumBonus = 0.0f;
	//
	int CurrentLevel = 0.0f;
	//
	//Array ind - achievement num, value - achievement level
	TArray<int> Achievements;

public:

	//Delegate for widget
	UPROPERTY(BlueprintAssignable, Category = "IncrementScore")
		FOnIncrementScore OnIncrementScore;

	//Function for increment score
	void IncrementScore(float MutableScore);

	//Function for incrementing NumBonus
	void IncrementNumBonus();

	//Function for check win condition (if we have score mechanics)
	void CheckWinCondition(float CurrentScore);

	//Calculating earn achivements;
	UFUNCTION(BlueprintCallable)
		TArray<FVector2D> GetEarnedAchievements();

	//Getter for achievements array
	UFUNCTION(BlueprintCallable)
		TArray<int> GetAchievements();
};
