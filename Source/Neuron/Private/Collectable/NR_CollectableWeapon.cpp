// Fill out your copyright notice in the Description page of Project Settings.

#include "Collectable/NR_CollectableWeapon.h"
#include "Character/NR_Character.h"
#include "Components/SphereComponent.h"
#include "Game/NR_PlayerController.h"


// Sets default values
ANR_CollectableWeapon::ANR_CollectableWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create RootComponent
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	//Create Static mesh
	StaticMeshWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshWeapon->SetGenerateOverlapEvents(false);
	StaticMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshWeapon->SetupAttachment(RootComponent);

	//Creating Collision Sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(34.f);
}

// Called when the game starts or when spawned
void ANR_CollectableWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANR_CollectableWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANR_CollectableWeapon::EnteredInteractionZone_Implementation(ANR_Character* Character)
{
	if (Character)
	{
		Character->AddNewWeaponToWeaponSlot(WeaponName);
	}
}
