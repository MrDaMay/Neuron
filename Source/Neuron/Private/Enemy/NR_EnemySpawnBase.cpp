// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NR_EnemySpawnBase.h"

// Sets default values
ANR_EnemySpawnBase::ANR_EnemySpawnBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANR_EnemySpawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANR_EnemySpawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

