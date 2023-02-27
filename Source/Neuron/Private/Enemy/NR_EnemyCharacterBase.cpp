// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NR_EnemyCharacterBase.h"

// Sets default values
ANR_EnemyCharacterBase::ANR_EnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANR_EnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANR_EnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANR_EnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

