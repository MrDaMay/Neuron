// Fill out your copyright notice in the Description page of Project Settings.


#include "Envinronment/NR_EnvinronmentObjects.h"
#include "Game/NR_GameState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANR_EnvinronmentObjects::ANR_EnvinronmentObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetCollisionProfileName("StaticObjects");
	StaticMesh->SetupAttachment(RootComponent);
}

void ANR_EnvinronmentObjects::ChangeObjectForBoss()
{
	GetWorldTimerManager().SetTimer(ChangePositionTimer, this, &ANR_EnvinronmentObjects::ChangePosition, 0.01f, true, 0.0f);
	GetWorldTimerManager().SetTimer(FinishChangePositionTimer, this, &ANR_EnvinronmentObjects::FinishChangePosition, 2.0f, false, 2.0f);
}

void ANR_EnvinronmentObjects::ChangePosition()
{
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + ZChangeOffset));
}

void ANR_EnvinronmentObjects::FinishChangePosition()
{
	GetWorldTimerManager().ClearTimer(ChangePositionTimer);
	GetWorldTimerManager().ClearTimer(FinishChangePositionTimer);

	if (ZChangeOffset < 0)
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void ANR_EnvinronmentObjects::BeginPlay()
{
	Super::BeginPlay();

	ZChangeOffset = (FinishZPosition - GetActorLocation().Z) / 200;

	Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()))->OnChangeMap.AddDynamic(this, &ANR_EnvinronmentObjects::ChangeObjectForBoss);

}

// Called every frame
void ANR_EnvinronmentObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

