// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable/NR_CollectableWeapons.h"
#include "Game/NR_PlayerController.h"

void ANR_CollectableWeapons::CollectItem_Implementation(ANR_Character* Character)
{
	if (Character)
	{
		Character->AddNewWeaponToWeaponSlot(ObjectName);
		//Character->InitWeapon(WeaponName);
		Destroy();
	}
}

void ANR_CollectableWeapons::EnteredInteractionZone_Implementation(ANR_Character* Character)
{
	if (Character)
	{
		auto Controller = Cast<ANR_PlayerController>(Character->GetController());
		if (Controller)
		{
			Controller->OpenClosePickupWidget(ObjectName);
		}
	}
}

void ANR_CollectableWeapons::LeftInteractionZone_Implementation(ANR_Character* Character)
{
	if (Character)
	{
		auto Controller = Cast<ANR_PlayerController>(Character->GetController());
		if (Controller)
		{
			Controller->OpenClosePickupWidget(ObjectName);
		}
	}
}
