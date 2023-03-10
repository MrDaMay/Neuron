// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable/NR_CollectableBase.h"
#include "NR_CollectableWeapons.generated.h"

/**
 * 
 */
UCLASS()
class NEURON_API ANR_CollectableWeapons : public ANR_CollectableBase
{
	GENERATED_BODY()

public:

	//Implementing interface
	void CollectItem_Implementation(ANR_Character* Character) override; // c++ implementation
	void EnteredInteractionZone_Implementation(ANR_Character* Character) override; // c++ implementation
	void LeftInteractionZone_Implementation(ANR_Character* Character) override; // c++ implementation
};
