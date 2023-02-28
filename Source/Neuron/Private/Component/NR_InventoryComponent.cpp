// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NR_InventoryComponent.h"
#include "Game/NR_GameInstance.h"

// Sets default values for this component's properties
UNR_InventoryComponent::UNR_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNR_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNR_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UNR_InventoryComponent::SwitchWeaponToIndex(int32 OldIndex, FAdditionalWeaponInfo OldInfo)
{
	bool bIsSuccess = false;
	int8 ChangedWeapon = 0;

	FName NewNameWeapon;
	FAdditionalWeaponInfo NewAdditionalInfo;

	int32 ChangeToIndex = OldIndex + 1;

	WeaponSlots[OldIndex].AdditionalInfo = OldInfo;

	while (!bIsSuccess && ChangedWeapon < WeaponSlots.Num())
	{
		if (ChangeToIndex == WeaponSlots.Num())
		{
			ChangeToIndex = 0;
		}

		if (!WeaponSlots[ChangeToIndex].NameItem.IsNone())
		{
			FWeaponInfo myInfo;

			UNR_GameInstance* myGi = Cast<UNR_GameInstance>(GetWorld()->GetGameInstance());

			if (myGi)
			{
				myGi->GetWeaponInfoByName(WeaponSlots[ChangeToIndex].NameItem, myInfo);

				int8 IndexWeaponAmmoSlot = 0;
				while (!bIsSuccess && IndexWeaponAmmoSlot < AmmoSlots.Num())
				{
					if (AmmoSlots[IndexWeaponAmmoSlot].WeaponType == myInfo.WeaponType)
					{
						if (AmmoSlots[IndexWeaponAmmoSlot].Cout != 0 || WeaponSlots[ChangeToIndex].AdditionalInfo.Round != 0)
						{
							NewNameWeapon = WeaponSlots[ChangeToIndex].NameItem;
							NewAdditionalInfo = WeaponSlots[ChangeToIndex].AdditionalInfo;
							bIsSuccess = true;
						}
					}
					IndexWeaponAmmoSlot++;
				}
			}
		}

		ChangedWeapon++;
		ChangeToIndex++;
	}

	if (bIsSuccess)
	{
		SetAdditionalInfoWeapon(OldIndex, OldInfo);
		SwitchWeapon(NewNameWeapon, NewAdditionalInfo, ChangeToIndex - 1);
	}

	return bIsSuccess;
}

FAdditionalWeaponInfo UNR_InventoryComponent::GetAdditionalInfoWeapon(int32 IndexWeapon)
{
	FAdditionalWeaponInfo result;

	if (WeaponSlots.IsValidIndex(IndexWeapon))
	{
		bool bIsFind = false;
		int8 i = 0;
		while (i < WeaponSlots.Num() && !bIsFind)
		{
			if (i == IndexWeapon)
			{
				result = WeaponSlots[i].AdditionalInfo;
				bIsFind = true;
			}
			i++;
		}
		if (!bIsFind)
			UE_LOG(LogTemp, Warning, TEXT(" FAdditionalWeaponInfo USnowBall_TPSInventoryComponent::GetAdditionalinfoWeapon - No found weapon with inex - %d"), IndexWeapon);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT(" FAdditionalWeaponInfo USnowBall_TPSInventoryComponent::GetAdditionalinfoWeapon - Not correct index weapon - %d"), IndexWeapon);

	return result;
}

int32 UNR_InventoryComponent::GetWeaponIndexSlotByName(FName IdWeapon)
{
	int32 result = -1;
	int8 i = 0;
	bool bIsFind = false;

	while (i < WeaponSlots.Num() && !bIsFind)
	{
		if (WeaponSlots[i].NameItem == IdWeapon)
		{
			result = i;
			bIsFind = true;
		}
		i++;
	}

	return result;
}

FName UNR_InventoryComponent::GetWeaponNameSlotByIndex(int32 IndexWeapon)
{
	if (WeaponSlots.IsValidIndex(IndexWeapon))
	{
		return WeaponSlots[IndexWeapon].NameItem;
	}
	return "";
}

void UNR_InventoryComponent::SetAdditionalInfoWeapon(int32 IndexWeapon, FAdditionalWeaponInfo NewInfo)
{
	if (WeaponSlots.IsValidIndex(IndexWeapon))
	{
		bool bIsFind = false;
		int8 i = 0;

		while (i < WeaponSlots.Num() && !bIsFind)
		{
			if (i == IndexWeapon)
			{
				WeaponSlots[i].AdditionalInfo = NewInfo;
				bIsFind = true;

				OnWeaponAdditionalInfoChange.Broadcast(IndexWeapon, NewInfo);
			}
			i++;
		}
		if (!bIsFind)
			UE_LOG(LogTemp, Warning, TEXT(" USnowBall_TPSInventoryComponent::SetAdditionalInfoWeapon - No found weapon with inex - %d"), IndexWeapon);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT(" USnowBall_TPSInventoryComponent::SetAdditionalInfoWeapon - Not correct index weapon - %d"), IndexWeapon);
}

void UNR_InventoryComponent::AmmoSlotChangeValue(EWeaponType WeaponType, int32 CoutChangeAmmo)
{
	bool bIsFind = false;
	int i = 0;

	while (i < AmmoSlots.Num() && !bIsFind)
	{
		if (AmmoSlots[i].WeaponType == WeaponType)
		{
			AmmoSlots[i].Cout += CoutChangeAmmo;

			if (AmmoSlots[i].Cout > AmmoSlots[i].MaxCout)
				AmmoSlots[i].Cout = AmmoSlots[i].MaxCout;

			if (AmmoSlots[i].Cout < 0)
			{
				AmmoSlots[i].Cout = 0;
			}

			OnAmmoChange.Broadcast(WeaponType, AmmoSlots[i].Cout);

			if (CoutChangeAmmo > 0)
				OnAddedAmmo.Broadcast(WeaponType);

			bIsFind = true;
		}
		i++;
	}
}

bool UNR_InventoryComponent::CheckAmmoForWeapon(EWeaponType TypeWeapon, int8& AvialbeAmmoForWeapon)
{
	AvialbeAmmoForWeapon = 0;

	bool bIsFind = false;
	int32 i = 0;

	while (i < AmmoSlots.Num() && !bIsFind)
	{
		if (AmmoSlots[i].WeaponType == TypeWeapon)
		{
			bIsFind = true;

			AvialbeAmmoForWeapon = AmmoSlots[i].Cout;

			if (AmmoSlots[i].Cout > 0)
				return true;
		}

		i++;
	}

	return false;
}

bool UNR_InventoryComponent::CheckForSimilarWeapon(FWeaponSlot NewWeapon)
{
	int i = 0;
	while (i < WeaponSlots.Num())
	{
		if (WeaponSlots[i].NameItem == NewWeapon.NameItem)
		{
			return false;
		}
		i++;
	}

	return true;
}

bool UNR_InventoryComponent::CheckCanTakeAmmo(EWeaponType AmmoType)
{
	bool result = false;
	int8 i = 0;
	while (i < AmmoSlots.Num() && !result)
	{
		if (AmmoSlots[i].WeaponType == AmmoType && AmmoSlots[i].Cout < AmmoSlots[i].MaxCout)
		{
			result = true;
		}

		i++;
	}
	return result;
}

bool UNR_InventoryComponent::CheckCanTakeWeapon(int32& FreeSlot)
{
	bool bIsFreeSlot = false;
	int8 i = 0;

	while (i < WeaponSlots.Num() && !bIsFreeSlot)
	{
		if (WeaponSlots[i].NameItem.IsNone())
		{
			FreeSlot = i;
			bIsFreeSlot = true;
		}
		i++;
	}
	return bIsFreeSlot;
}

TArray<FWeaponSlot> UNR_InventoryComponent::GetWeaponSlots()
{
	return WeaponSlots;
}

TArray<FAmmoSlot> UNR_InventoryComponent::GetAmmoSlots()
{
	return AmmoSlots;
}

void UNR_InventoryComponent::SwitchWeapon(FName NewWeaponNamee, FAdditionalWeaponInfo WeaponAdditionalInfo,
	int32 NewCurrentIndexWeapon)
{
	OnSwichWeapon.Broadcast(NewWeaponNamee, WeaponAdditionalInfo, NewCurrentIndexWeapon);
}

void UNR_InventoryComponent::InitInventory(const TArray<FWeaponSlot>& NewWeaponSlots,
	const TArray<FAmmoSlot>& NewAmmoSlots)
{
	WeaponSlots = NewWeaponSlots;
	AmmoSlots = NewAmmoSlots;

	MaxSlotWeapon = WeaponSlots.Num();

	if (WeaponSlots.IsValidIndex(0))
	{
		if (!WeaponSlots[0].NameItem.IsNone())
		{
			SwitchWeapon(WeaponSlots[0].NameItem, WeaponSlots[0].AdditionalInfo, 0);
		}
	}
}


