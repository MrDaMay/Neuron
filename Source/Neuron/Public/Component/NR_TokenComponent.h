// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FuncLibrary/Type.h"
#include "NR_TokenComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTokenChanged, FVector2D, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTokensUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEURON_API UNR_TokenComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNR_TokenComponent();

	// Delegates for widget updates
	UPROPERTY(BlueprintAssignable)
	FOnTokenChanged OnTokenChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnTokensUpdated OnTokensUpdated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//*****************************
	//// Pointers to dependable objects
	//class UCharacterMovementComponent* Movement = nullptr;
	//class ANR_Weapon* Weapon = nullptr;
	//class UNR_HealthComponent* Health = nullptr;
	//*****************************

	int length = static_cast<int>(ETokenType::NUMBER_OF_TOKENS);
	//Tokens array
	TArray<int32> Tokens;

	//*****************************
	////Update Stats corresponding to tokens
	//void UpdateStats(int idx);
	////Function for changing value of stat
	//float NewStatValue(float OldStat, int TokenIdx, float Multiplier);
	////Bool for checking increase/decrease
	//bool bIsIncreasing = false;
	//*****************************

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	////InitTokenComponent
	//UFUNCTION(BlueprintCallable)
	//void InitTokens(UCharacterMovementComponent* MovementPtr, ANR_Weapon* WeaponPtr, UNR_HealthComponent* HealthPtr);

	//InitTokenComponent
	UFUNCTION(BlueprintCallable)
	void InitTokens(int InitValue = 0);

	//Put Token at idx
	UFUNCTION(BlueprintCallable)
	void CollectToken(int idx);

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetTokenArray();

	UFUNCTION(BlueprintCallable)
	void CopyTokenArray(TArray<int32> Array);
	
	//*****************************
	////Retrive Token at idx
	//UFUNCTION(BlueprintCallable)
	//void RetrieveTokenFrom (int idx);
	//*****************************
	// 
	//*****************************
	////Update one of the stats
	//UFUNCTION(BlueprintCallable)
	//void UpdateWeaponDamage();
	//UFUNCTION(BlueprintCallable)
	//void UpdateWeaponRateOfFire();
	//UFUNCTION(BlueprintCallable)
	//void UpdateMaxHealth();
	//UFUNCTION(BlueprintCallable)
	//void UpdateMovementSpeed();
	//*****************************

};
