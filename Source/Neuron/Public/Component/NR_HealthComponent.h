// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NR_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEURON_API UNR_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNR_HealthComponent();

	//Delegate for widget
	UPROPERTY(BlueprintCallable)
	FOnHealthChange OnHealthChange;
	//Delegate for dead
	FOnDead OnDead;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultHealth = 100.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float Health = 0.0f;
	float MaxHealth = 0.0f;
	bool IsAlive = true;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Function for check alive 
	UFUNCTION(BlueprintCallable, Category = "Health")
		bool GetIsAlive();
	//Max Health getter  
	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetMaxHealth() { return MaxHealth;  };
	//Function increment Health
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ChangeHealthValue(float ChangeValue);
};
