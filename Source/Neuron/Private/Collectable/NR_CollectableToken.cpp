// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable/NR_CollectableToken.h"

void ANR_CollectableToken::EnteredInteractionZone_Implementation(ANR_Character* Character)
{
	Super::EnteredInteractionZone_Implementation(Character);

	Character->TakeToken(ObjectName);
	Destroy();
}
