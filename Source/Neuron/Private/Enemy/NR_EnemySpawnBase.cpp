// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NR_EnemySpawnBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/NR_GameMode.h"

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
	GameMode->OnSpawnEnemy.AddDynamic(this, &ANR_EnemySpawnBase::SpawnEnemy);
}

// Called every frame
void ANR_EnemySpawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANR_EnemySpawnBase::SpawnEnemy()
{
	FVector BoxSize = FVector(BoxComponent->GetScaledBoxExtent().X/2, BoxComponent->GetScaledBoxExtent().Y / 2, 0.0f);
	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxSize);
	FRotator SpawnRotation = FRotator(0);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	int32 EnemyIndex = UKismetMathLibrary::RandomInteger64InRange(0, Enemy.Num() - 1);

	GetWorld()->SpawnActor(Enemy[EnemyIndex], &SpawnLocation, &SpawnRotation, SpawnParameters);
}