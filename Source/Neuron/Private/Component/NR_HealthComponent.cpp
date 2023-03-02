// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NR_HealthComponent.h"
#include "Game/NR_PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UNR_HealthComponent::UNR_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNR_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	//Delegat for respawn
	auto PlayerController = Cast<ANR_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->OnRespawn.AddDynamic(this, &UNR_HealthComponent::RefreshParams);
	
}


// Called every frame
void UNR_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UNR_HealthComponent::GetIsAlive()
{
	return IsAlive;
}

void UNR_HealthComponent::ChangeHealthValue(float ChangeValue)
{
	//Check alive
	if (IsAlive == true)
	{
		//Increment health
		Health += ChangeValue;


		if (Health > 100.0f)
		{
			Health = 100.0f;
		}
		else
		{
			if (Health <= 0.0f)
			{
				Health = 0.0f;
				OnDead.Broadcast();
				IsAlive = false;
			}
		}

		//Say widget what health changed
		OnHealthChange.Broadcast(Health, ChangeValue);
	}
}

void UNR_HealthComponent::RefreshParams()
{
	Health = 100.0f;

	OnHealthChange.Broadcast(Health, 0);

	GetWorld()->GetTimerManager().SetTimer(RefreshAliveTimer, this, &UNR_HealthComponent::RefreshAlive, 5.0f, true, 5.0f);
}

void UNR_HealthComponent::RefreshAlive()
{
	IsAlive = true;
}

