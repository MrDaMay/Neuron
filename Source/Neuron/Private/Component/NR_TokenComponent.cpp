// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/NR_Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/NR_HealthComponent.h"
#include "Component/NR_TokenComponent.h"

// Sets default values for this component's properties
UNR_TokenComponent::UNR_TokenComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Tokens.Init(0, length);
	// ...
}


// Called when the game starts
void UNR_TokenComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//void UNR_TokenComponent::UpdateStats(int idx)
//{
//	switch (idx)
//	{
//		case 0:
//			UpdateWeaponDamage();
//			break;
//		case 1:
//			UpdateMaxHealth();
//			break;
//		case 2:
//			UpdateWeaponRateOfFire();
//			break;
//		case 3:
//			UpdateMovementSpeed();
//			break;
//		case 4:
//			break;
//		case 5:
//			break;
//	}
//	// ...
//
//}

//// !!Need to ReDO!!
//float UNR_TokenComponent::NewStatValue(float OldStat, int TokenIdx, float Multiplier)
//{
//	float buff = 0;
//	if (bIsIncreasing)
//	{
//		if (TokensArray[TokenIdx] == 1)
//		{
//			buff = OldStat * (1 + TokensArray[TokenIdx] * Multiplier);
//		} // calculating new value
//		else
//		{
//			buff = OldStat / (1 + (TokensArray[TokenIdx] - 1) * Multiplier); // reseting to the default value by normalization to the prevoius Tokens quantity
//			buff *= (1 + TokensArray[TokenIdx] * Multiplier); // calculating new value
//		}
//	}
//	else
//	{
//		buff = OldStat / (1 + (TokensArray[TokenIdx] + 1) * Multiplier);  // reseting to the default value by normalization to the prevoius Tokens quantity
//		buff *= (1 + TokensArray[TokenIdx] * Multiplier); // calculating new value
//	}
//
//	return buff;
//}


// Called every frame
void UNR_TokenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNR_TokenComponent::InitTokens(int InitValue)
{
	//Initializing array with zero
	for (auto& Token : Tokens)
	{
		Token = InitValue;
	}

}

void UNR_TokenComponent::CollectToken(ETokenType Token)
{
	int idx = static_cast<int>(Token);
	Tokens[idx] += 1;
	//OnTokenChangedDelegate.Broadcast(TokensArray[idx]); // Broadcasting new number of idx token
}

TArray<int32> UNR_TokenComponent::GetTokenArray()
{
	return Tokens;
}

void UNR_TokenComponent::CopyTokenArray(TArray<int32> Array)
{
	for (int i = 0; i < length; ++i)
	{
		Tokens[i] = Array[i];
	}
}

//void UNR_TokenComponent::RetrieveTokenFrom(int idx)
//{
//	if (TokensArray[idx] > 0)
//	{
//		bIsIncreasing = false;
//		TokensArray[idx] -= 1;
//		// ...
//		UpdateStats(idx);
//		OnTokenChangedDelegate.Broadcast(TokensArray[idx]); // Broadcasting new number of idx token
//	}
//
//}
//
//void UNR_TokenComponent::UpdateWeaponDamage()
//{
//	// Damage of Weapon??
//}
//
//void UNR_TokenComponent::UpdateWeaponRateOfFire()
//{
//	Weapon->WeaponSetting.RateOfFire = NewStatValue(Weapon->WeaponSetting.RateOfFire, TokensArray[2], 0.02f);
//}
//
//void UNR_TokenComponent::UpdateMaxHealth()
//{
//	// MaxHealth field in HealthComponent??
//}
//
//void UNR_TokenComponent::UpdateMovementSpeed()
//{
//	Movement->MaxWalkSpeed = NewStatValue(Movement->MaxWalkSpeed, TokensArray[3], 0.05f);
//}