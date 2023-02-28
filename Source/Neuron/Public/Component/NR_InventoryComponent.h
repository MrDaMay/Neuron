// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FuncLibrary/Type.h"
#include "NR_InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSwitchWeapon, FName, NewWeaponNamee, FAdditionalWeaponInfo, WeaponAdditionalInfo, int32, NewCurrentIndexWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange, EWeaponType, TypeAmmo, int32, CoutAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponAdditionalInfoChange, int32, IndexSlot, FAdditionalWeaponInfo, AdditionalInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateWeaponSlots, int32, IndexSlotChange, FWeaponSlot, NewInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoTypeEmpty, EWeaponType, WeapoType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddedAmmo, EWeaponType, WeapoType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEURON_API UNR_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNR_InventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnSwitchWeapon OnSwichWeapon;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnAmmoChange OnAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnWeaponAdditionalInfoChange OnWeaponAdditionalInfoChange;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnUpdateWeaponSlots OnUpdateWeaponSlots;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnAmmoTypeEmpty OnAmmoTypeEmpty;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnAddedAmmo OnAddedAmmo;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
		TArray<FWeaponSlot> WeaponSlots;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
		TArray<FAmmoSlot> AmmoSlots;

	int32 MaxSlotWeapon = 0;

	bool SwitchWeaponToIndex(int32 OldIndex, FAdditionalWeaponInfo OldInfo);

	FAdditionalWeaponInfo GetAdditionalInfoWeapon(int32 IndexWeapon);
	int32 GetWeaponIndexSlotByName(FName IdWeapon);
	FName GetWeaponNameSlotByIndex(int32 IndexWeapon);

	void SetAdditionalInfoWeapon(int32 IndexWeapon, FAdditionalWeaponInfo NewInfo);

	UFUNCTION(BlueprintCallable, Category = "Intrface")
		void AmmoSlotChangeValue(EWeaponType WeaponType, int32 CoutChangeAmmo);

	bool CheckAmmoForWeapon(EWeaponType TypeWeapon, int8& AvialbeAmmoForWeapon);

	bool CheckForSimilarWeapon(FWeaponSlot NewWeapon);

	//Interface Pickup Actors
	UFUNCTION(BlueprintCallable, Category = "Intrface")
		bool CheckCanTakeAmmo(EWeaponType AmmoType);
	UFUNCTION(BlueprintCallable, Category = "Intrface")
		bool CheckCanTakeWeapon(int32& FreeSlot);

	//UFUNCTION(BlueprintCallable, Category = "Intrface")
	//	void TryGetWeaponToInventory(* PickUp, FWeaponSlot NewWeapon);
	UFUNCTION(BlueprintCallable, Category = "Inventary")
		TArray<FWeaponSlot> GetWeaponSlots();
	UFUNCTION(BlueprintCallable, Category = "Inventary")
		TArray<FAmmoSlot> GetAmmoSlots();

	UFUNCTION(BlueprintCallable, Category = "Inventary")
		void SwitchWeapon(FName NewWeaponNamee, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon);
	UFUNCTION(BlueprintCallable, Category = "Inventary")
		void InitInventory(const TArray<FWeaponSlot>& NewWeaponSlots, const TArray<FAmmoSlot>& NewAmmoSlots);
};
