// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NR_EnemyCharacterShooter.h"
#include "Game/NR_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void ANR_EnemyCharacterShooter::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

void ANR_EnemyCharacterShooter::SpawnWeapon()
{
	UNR_GameInstance* GameInstance = Cast<UNR_GameInstance>(GetGameInstance());
	FWeaponInfo WeaponInfo;

	if (GameInstance)
	{
		if (GameInstance->GetWeaponInfoByName("EnemyRifle", WeaponInfo))
		{
			if (WeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);

				FActorSpawnParameters SpawmParams;
				SpawmParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawmParams.Owner = this;
				SpawmParams.Instigator = GetInstigator();

				ANR_Weapon* Weapon = Cast<ANR_Weapon>(GetWorld()->SpawnActor(WeaponInfo.WeaponClass, &SpawnLocation,
					&SpawnRotation, SpawmParams));

				if (Weapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					Weapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocketRightHand"));
					CurrentWeapon = Weapon;

					Weapon->IsPlayer = false;
					Weapon->Pawn = Cast<ANR_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));;
					Weapon->WeaponSetting = WeaponInfo;
				}
			}
		}
	}
}

void ANR_EnemyCharacterShooter::CharacterDead()
{
	Super::CharacterDead();

	CurrentWeapon->Destroy();
}

void ANR_EnemyCharacterShooter::AttackEnemy(FName NotifyName,
                                            const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "Attack")
	{
		CurrentWeapon->FireProjectile();
	}
}
