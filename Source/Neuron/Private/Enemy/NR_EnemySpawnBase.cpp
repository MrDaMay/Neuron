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

	GameState = Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->OnBossPhaseStarts.AddDynamic(this, &ANR_EnemySpawnBase::SpawnBoss);
	OnSpawnEnd.AddDynamic(GameState, &ANR_GameState::EndWavePhase);

	AddSpawnBase();
}

// Called every frame
void ANR_EnemySpawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANR_EnemySpawnBase::SpawnBoss(TSubclassOf<class ANR_EnemyBoss> BossObject)
{
	FVector BoxSize = FVector(BoxComponent->GetScaledBoxExtent().X / 2, BoxComponent->GetScaledBoxExtent().Y / 2, 0.0f);
	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxSize);
	FRotator SpawnRotation = FRotator(0);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	auto Boss = Cast<ANR_EnemyBoss>(GetWorld()->SpawnActor(BossObject, &SpawnLocation, &SpawnRotation, SpawnParameters));
	if (Boss)
	{
		GameState->ToggleBossState();
	}
}

void ANR_EnemySpawnBase::SpawnEnemy(TSubclassOf<ANR_EnemyCharacterBase> EnemyObject)
{

	FVector BoxSize = GetActorLocation();

	if (IsValid(BoxComponent))
		BoxSize = FVector(BoxComponent->GetScaledBoxExtent().X / 2, BoxComponent->GetScaledBoxExtent().Y / 2, 0.0f);
	else
		UE_LOG(LogTemp, Warning, TEXT("ANR_EnemySpawnBase::SpawnEnemy, BoxComponent - null"));

	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxSize);
	FRotator SpawnRotation = FRotator(0);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	GetWorld()->SpawnActor(EnemyObject, &SpawnLocation, &SpawnRotation, SpawnParameters);
}

void ANR_EnemySpawnBase::AddSpawnBase()
{
	GameState->EnemySpawnBase.Add(this);
}
