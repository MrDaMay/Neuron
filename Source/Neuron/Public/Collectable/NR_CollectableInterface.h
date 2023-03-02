// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/NR_Character.h"
#include "NR_CollectableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNR_CollectableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEURON_API INR_CollectableInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//Interaction for picking up an item
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collect")
		void CollectItem(ANR_Character* Character); // Blueprint editable
	//Interering an interactive zone
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collect|Interaction")
		void EnteredInteractionZone(ANR_Character* Character); // Blueprint editable
	//Leaving an interactive zone
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Collect|Interaction")
		void LeftInteractionZone(ANR_Character* Character); // Blueprint editable
};