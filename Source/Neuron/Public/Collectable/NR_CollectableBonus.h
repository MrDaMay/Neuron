// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable/NR_CollectableBase.h"
#include "NR_CollectableBonus.generated.h"

/**
 * 
 */
UCLASS()
class NEURON_API ANR_CollectableBonus : public ANR_CollectableBase
{
	GENERATED_BODY()
	
public:

	void EnteredInteractionZone_Implementation(ANR_Character* Character) override; // c++ implementation
};
