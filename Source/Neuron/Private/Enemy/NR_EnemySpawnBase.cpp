// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NR_EnemySpawnBase.h"
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

	auto GameMode = Cast<ANR_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnSpawnEnemy.AddDynamic(this, &ANR_EnemySpawnBase::StartSpawn);
	auto GameState = Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->OnBossPhaseStarts.AddDynamic(this, &ANR_EnemySpawnBase::SpawnBoss);

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

	GetWorld()->SpawnActor(Enemy[3], &SpawnLocation, &SpawnRotation, SpawnParameters);
}

void ANR_EnemySpawnBase::SpawnEnemy()
{
	if (--RepeatingCallsRemaining <= 0)
	{
		StopSpawn();
	}
	FVector BoxSize = FVector(BoxComponent->GetScaledBoxExtent().X / 2, BoxComponent->GetScaledBoxExtent().Y / 2, 0.0f);
	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxSize);
	FRotator SpawnRotation = FRotator(0);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	int32 EnemyIndex = UKismetMathLibrary::RandomInteger64InRange(0, Enemy.Num() - 1);
	if (GetWorld()->SpawnActor(Enemy[EnemyIndex], &SpawnLocation, &SpawnRotation, SpawnParameters))
	{
		auto GameState = Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()));
		GameState->IncrementEnemies();
	}
}

void ANR_EnemySpawnBase::StartSpawn()
{
	auto GameState = Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->ToggleSpawningState();

	RepeatingCallsRemaining = EnemiesNumber;
	GetWorldTimerManager().SetTimer(SpawnEnemyTimer, this, &ANR_EnemySpawnBase::SpawnEnemy, SpawnRate, true, SpawnDelay);
}

void ANR_EnemySpawnBase::StopSpawn()
{
	GetWorldTimerManager().ClearTimer(SpawnEnemyTimer);
	auto GameState = Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->ToggleSpawningState();
	OnSpawnEnd.Broadcast();
}