// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable/NR_CollectableWeapons.h"
#include "Game/NR_PlayerController.h"


void ANR_CollectableWeapons::EnteredInteractionZone_Implementation(ANR_Character* Character)
{
	Super::EnteredInteractionZone_Implementation(Character);

	Character->AddNewWeaponToWeaponSlot(ObjectName);

	Destroy();
}