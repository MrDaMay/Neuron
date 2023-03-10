// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable/NR_CollectableBonus.h"

void ANR_CollectableBonus::EnteredInteractionZone_Implementation(ANR_Character* Character)
{
	Character->TakeBonus(ObjectName);
	Destroy();
}
