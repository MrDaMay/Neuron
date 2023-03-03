// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NR_FreezeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNR_FreezeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEURON_API INR_FreezeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void Freeze();
};
