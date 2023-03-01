// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NR_Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ANR_Projectile::ANR_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));

	BulletCollisionSphere->SetSphereRadius(16.f);
	BulletCollisionSphere->SetEnableGravity(false);

	BulletCollisionSphere->bReturnMaterialOnMove = true;//hit event return physMaterial

	BulletCollisionSphere->SetCanEverAffectNavigation(false);//collision not affect navigation (P keybord on editor)

	RootComponent = BulletCollisionSphere;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Projectile Mesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCanEverAffectNavigation(false);
	BulletMesh->SetEnableGravity(false);
	BulletMesh->SetCollisionProfileName("NoCollosion");

	BulletFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Bullet FX"));
	BulletFX->SetupAttachment(RootComponent);

	//BulletSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Bullet Audio"));
	//BulletSound->SetupAttachment(RootComponent);

	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Bullet ProjectileMovement"));
	BulletProjectileMovement->UpdatedComponent = RootComponent;
	BulletProjectileMovement->ProjectileGravityScale = 0.f;

	BulletProjectileMovement->bRotationFollowsVelocity = true;
	BulletProjectileMovement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void ANR_Projectile::BeginPlay()
{
	Super::BeginPlay();

	BulletCollisionSphere->OnComponentHit.AddDynamic(this, &ANR_Projectile::BulletCollisionSphereHit);
}

// Called every frame
void ANR_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANR_Projectile::InitProjectile(FProjectileInfo InitParam)
{
	this->SetLifeSpan(InitParam.ProjectileLifeTime);

	if (InitParam.ProjectileStaticMesh)
	{
		BulletMesh->SetStaticMesh(InitParam.ProjectileStaticMesh);
		BulletMesh->SetRelativeTransform(InitParam.ProjectileStaticMeshOffset);
	}
	else
		BulletMesh->DestroyComponent();

	if (InitParam.ProjectileTrailFx)
	{
		BulletFX->SetTemplate(InitParam.ProjectileTrailFx);
		BulletFX->SetRelativeTransform(InitParam.ProjectileTrailFxOffset);
	}
	else
		BulletFX->DestroyComponent();

	BulletProjectileMovement->Velocity = GetActorForwardVector() * InitParam.ProjectInitSpeed;
	BulletProjectileMovement->MaxSpeed = InitParam.ProjectMaxSpeed;

	ProjectileSetting = InitParam;
}

void ANR_Projectile::ImpactProjectile()
{
	this->Destroy();
}

void ANR_Projectile::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && Hit.PhysMaterial.IsValid())
	{
		EPhysicalSurface mySurfacetype = UGameplayStatics::GetSurfaceType(Hit);

		if (ProjectileSetting.HitDecal)
		{
			UMaterialInterface* Material = ProjectileSetting.HitDecal;

			if (Material && OtherComp)
			{
				UGameplayStatics::SpawnDecalAttached(Material, FVector(20.0f), OtherComp, NAME_None, Hit.ImpactPoint,
					Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
			}
		}
		if (ProjectileSetting.HitFXs)
		{
			UParticleSystem* Particle = ProjectileSetting.HitFXs;

			if (Particle)
			{
				UGameplayStatics::SpawnEmitterAttached(Particle, OtherComp, NAME_None, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(),
					EAttachLocation::KeepWorldPosition);
			}
		}
		if (ProjectileSetting.HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.HitSound, Hit.ImpactPoint, 0.3f);
		}


		UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileSetting.ProjectileDamage, Hit.TraceStart, Hit, GetInstigatorController(), this, NULL);

		ImpactProjectile();
	}
}

void ANR_Projectile::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	if (BulletProjectileMovement)
	{
		BulletProjectileMovement->Velocity = NewVelocity;
	}
}

