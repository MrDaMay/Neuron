// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FuncLibrary/Type.h"
#include "GameFramework/Actor.h"
#include "NR_Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReloadStart, UAnimMontage*, Anim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponReloadEnd, bool, bIsSuccess, int32, AmmoSpent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFireStart, UAnimMontage*, AnimMontage);

UCLASS()
class NEURON_API ANR_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANR_Weapon();

	FOnWeaponReloadStart OnWeaponReloadStart;
	FOnWeaponReloadEnd OnWeaponReloadEnd;
	FOnWeaponFireStart OnWeaponFireStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USkeletalMeshComponent* SkeletalMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UStaticMeshComponent* StaticMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UArrowComponent* ShootLocation = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Params start
	FProjectileInfo GetProjectile();

	class ANR_Character* Pawn = nullptr;
	FWeaponInfo WeaponSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponRateOfFire = 0.0f;
	//Params end

	//Fire start
	UFUNCTION()
		void FireButtonPressed(bool bIsFire);
	void Fire();
	//Fire end

	//Shoot start
	void FindEndLocation();
	FVector ShotEndLocation = FVector(0.0f);
	float SizeChanelToChangeShootDirectionLogic = 120.0f;
	float CoefDispersion = 0.0f;
	//Shoot end

	//Timers start
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireTimer = 100.0f;
	float ReloadTimer = 0.0f;
	FTimerHandle FireTimerHandle;
	//Timers end

	//Confusion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireLogic")
		int Confusion = 2;
};
