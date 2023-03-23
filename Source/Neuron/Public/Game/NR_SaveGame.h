// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NR_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NEURON_API UNR_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	//Achivments
	TArray<float> Achievements;
	//Tokens
	TArray<int> Tokens;
	//CharacterWeapons
	TArray<FName> Weapons;
};
