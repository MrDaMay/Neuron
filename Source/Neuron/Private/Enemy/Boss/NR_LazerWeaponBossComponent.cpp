// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/NR_LazerWeaponBossComponent.h"

// Sets default values for this component's properties
UNR_LazerWeaponBossComponent::UNR_LazerWeaponBossComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNR_LazerWeaponBossComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNR_LazerWeaponBossComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

