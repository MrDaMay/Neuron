// Fill out your copyright notice in the Description page of Project Settings.


#include "Envinronment/NR_EnvinronmentLight.h"

#include "Components/PointLightComponent.h"
#include "Game/NR_GameState.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANR_EnvinronmentLight::ANR_EnvinronmentLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetVisibility(true);
	PointLight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANR_EnvinronmentLight::BeginPlay()
{
	Super::BeginPlay();

	Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()))->OnWavePhaseEnds.AddDynamic(this, &ANR_EnvinronmentLight::ChangeObjectForBoss);
}

void ANR_EnvinronmentLight::ChangeObjectForBoss()
{
	PointLight->LightColor = BossColor;
}

// Called every frame
void ANR_EnvinronmentLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

