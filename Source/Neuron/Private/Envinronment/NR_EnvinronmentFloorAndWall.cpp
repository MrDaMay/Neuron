// Fill out your copyright notice in the Description page of Project Settings.


#include "Envinronment/NR_EnvinronmentFloorAndWall.h"
#include "Game/NR_GameState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANR_EnvinronmentFloorAndWall::ANR_EnvinronmentFloorAndWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetCollisionProfileName("StaticObjects");
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ANR_EnvinronmentFloorAndWall::BeginPlay()
{
	Super::BeginPlay();

	Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()))->OnChangeMap.AddDynamic(this, &ANR_EnvinronmentFloorAndWall::ChangeObjectForBoss);
}


void ANR_EnvinronmentFloorAndWall::ChangeObjectForBoss()
{
	StaticMesh->SetMaterial(0, BossMaterial);
}

// Called every frame
void ANR_EnvinronmentFloorAndWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

