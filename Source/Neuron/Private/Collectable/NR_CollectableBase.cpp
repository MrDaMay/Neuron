// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable/NR_CollectableBase.h"

// Sets default values
ANR_CollectableBase::ANR_CollectableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANR_CollectableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANR_CollectableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANR_CollectableBase::CollectItem_Implementation(ANR_Character* Character)
{

}

void ANR_CollectableBase::EnteredInteractionZone_Implementation(ANR_Character* Character)
{

}

void ANR_CollectableBase::LeftInteractionZone_Implementation(ANR_Character* Character)
{

}