// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable/NR_CollectableBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ANR_CollectableBase::ANR_CollectableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetSphereRadius(60.f);
	CollisionSphere->SetEnableGravity(false);
	CollisionSphere->bReturnMaterialOnMove = true;//hit event return physMaterial
	CollisionSphere->SetCanEverAffectNavigation(false);//collision not affect navigation (P keybord on editor)
	CollisionSphere->SetCollisionProfileName("OverlapAll");

	RootComponent = CollisionSphere;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCanEverAffectNavigation(false);
	StaticMesh->SetEnableGravity(false);
	StaticMesh->SetCollisionProfileName("NoCollision");
	StaticMesh->SetRenderCustomDepth(true);
	StaticMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 62.0f));
	StaticMesh->CustomDepthStencilValue = 3;

	LightPartical = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Partical"));
	LightPartical->SetupAttachment(RootComponent);

	

}

// Called when the game starts or when spawned
void ANR_CollectableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANR_CollectableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANR_CollectableBase::CollectItem_Implementation(ANR_Character* Character)
{

}

void ANR_CollectableBase::EnteredInteractionZone_Implementation(ANR_Character* Character)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapSpawnPartical, GetActorLocation(),
		FRotator(0));

	if(PickUpSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, GetActorLocation());
}

void ANR_CollectableBase::LeftInteractionZone_Implementation(ANR_Character* Character)
{

}