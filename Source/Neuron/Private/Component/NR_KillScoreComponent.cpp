// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NR_KillScoreComponent.h"
#include "Game/NR_PlayerState.h"
#include "Game/NR_GameState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UNR_KillScoreComponent::UNR_KillScoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNR_KillScoreComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNR_KillScoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNR_KillScoreComponent::OwnerWasDead()
{
	ANR_PlayerState* PlayerState = Cast<ANR_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));

	PlayerState->IncrementScore(1); //ToDo need added score from funclibrary and table

	auto GameState = Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->TryToChangePhase();
}

