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
	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;
	//Delegate for dead
	FOnDead OnDead;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float Health = 100.0f;
	bool IsAlive = true;

	FTimerHandle RefreshAliveTimer;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float CoefDamageResist = 1.0f;

	//Function for check alive 
	UFUNCTION(BlueprintCallable, Category = "Health")
		bool GetIsAlive();

	//Function increment Health
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ChangeHealthValue(float ChangeValue);
};
