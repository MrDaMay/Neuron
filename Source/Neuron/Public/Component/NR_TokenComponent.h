// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NR_TokenComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTokenChanged, int, Idx);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEURON_API UNR_TokenComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNR_TokenComponent();

	// Deelgate for widget updates
	UPROPERTY(BlueprintAssignable)
	FOnTokenChanged OnTokenChangedDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Pointers to dependable objects
	class UCharacterMovementComponent* Movement = nullptr;
	class ANR_Weapon* Weapon = nullptr;
	class UNR_HealthComponent* Health = nullptr;

	//Tokens array
	float TokensArray[6] = { 0, 0, 0, 0, 0, 0 };
	//Update Stats corresponding to tokens
	void UpdateStats(int idx);
	//Function for changing value of stat
	float NewStatValue(float OldStat, int TokenIdx, float Multiplier);
	//Bool for checking increase/decrease
	bool bIsIncreasing = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//InitTokenComponent
	UFUNCTION(BlueprintCallable)
	void InitTokens(UCharacterMovementComponent* MovementPtr, ANR_Weapon* WeaponPtr, UNR_HealthComponent* HealthPtr);

	//Put Token at idx
	UFUNCTION(BlueprintCallable)
	void PutTokenAt (int idx);
	//Retrive Token at idx
	UFUNCTION(BlueprintCallable)
	void RetrieveTokenFrom (int idx);

	//Update one of the stats
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponDamage();
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponRateOfFire();
	UFUNCTION(BlueprintCallable)
	void UpdateMaxHealth();
	UFUNCTION(BlueprintCallable)
	void UpdateMovementSpeed();


};