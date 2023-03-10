// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NR_EnemySpawnBase.h"
#include "Enemy/Boss/NR_EnemyBoss.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/NR_GameMode.h"
#include "Game/NR_GameState.h"
#include "Game/NR_PlayerController.h"

// Sets default values
ANR_EnemySpawnBase::ANR_EnemySpawnBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->InitBoxExtent(FVector(20.0f, 20.0f, 10.0f));
}

// Called when the game starts or when spawned
void ANR_EnemySpawnBase::BeginPlay()
{
	Super::BeginPlay();

	MyGameState = Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	MyGameState->OnWavePhaseStarts.AddDynamic(this, &ANR_EnemySpawnBase::StartSpawn);
	MyGameState->OnBossPhaseStarts.AddDynamic(this, &ANR_EnemySpawnBase::SpawnBoss);
	OnSpawnEnd.AddDynamic(MyGameState, &ANR_GameState::EndWavePhase);

	//auto Controller = Cast<ANR_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//OnSpawnEnd.AddDynamic(Controller, &ANR_PlayerController::UnblockTokenWidget);
}

// Called every frame
void ANR_EnemySpawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANR_EnemySpawnBase::SpawnBoss()
{
	FVector BoxSize = FVector(BoxComponent->GetScaledBoxExtent().X / 2, BoxComponent->GetScaledBoxExtent().Y / 2, 0.0f);
	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxSize);
	FRotator SpawnRotation = FRotator(0);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	auto Boss = Cast<ANR_EnemyBoss>(GetWorld()->SpawnActor(Enemy[3], &SpawnLocation, &SpawnRotation, SpawnParameters));
	if (Boss)
	{
		MyGameState->ToggleBossState();
	}
}

void ANR_EnemySpawnBase::SpawnEnemy()
{
	FVector BoxSize = FVector(BoxComponent->GetScaledBoxExtent().X / 2, BoxComponent->GetScaledBoxExtent().Y / 2, 0.0f);
	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxSize);
	FRotator SpawnRotation = FRotator(0);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	int32 EnemyIndex = UKismetMathLibrary::RandomInteger64InRange(0, Enemy.Num() - 2);
	if (GetWorld()->SpawnActor(Enemy[EnemyIndex], &SpawnLocation, &SpawnRotation, SpawnParameters))
	{
		MyGameState->IncrementEnemies();
	}

	if (RepeatingCallsRemaining-- <= 0)
	{
		StopSpawn();
	}
}

void ANR_EnemySpawnBase::StartSpawn()
{
	MyGameState->ToggleSpawningState();

	RepeatingCallsRemaining = EnemiesNumber;
	GetWorldTimerManager().SetTimer(SpawnEnemyTimer, this, &ANR_EnemySpawnBase::SpawnEnemy, SpawnRate, true, SpawnDelay);
}

void ANR_EnemySpawnBase::StopSpawn()
{
	GetWorldTimerManager().ClearTimer(SpawnEnemyTimer);

	MyGameState->ToggleSpawningState();
	OnSpawnEnd.Broadcast();
}