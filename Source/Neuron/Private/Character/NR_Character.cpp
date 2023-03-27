// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NR_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/NR_GameInstance.h"
#include "Game/NR_GameState.h"
#include "Game/NR_PlayerState.h"
#include "Game/NR_PlayerController.h"
#include "Game/NR_GameInstance.h"
#include "Enemy/NR_FreezeInterface.h"

void ANR_Character::RollReload()
{
	bIsRollReady = true;
	OnRollReloadEnd.Broadcast();
}

void ANR_Character::SwitchToWeaponOne()
{
	if(WeaponSLot.IsValidIndex(0))
	{
		if(WeaponSLot[0] != CurrentWeapon->WeaponName)
			InitWeapon(WeaponSLot[0]);
	}
}

void ANR_Character::SwitchToWeaponTwo()
{
	if (WeaponSLot.IsValidIndex(1))
	{
		if (WeaponSLot[1] != CurrentWeapon->WeaponName)
			InitWeapon(WeaponSLot[1]);
	}
}

void ANR_Character::SwitchToWeaponThree()
{
	if (WeaponSLot.IsValidIndex(2))
	{
		if (WeaponSLot[2] != CurrentWeapon->WeaponName)
			InitWeapon(WeaponSLot[2]);
	}
}

void ANR_Character::SwitchToWeaponFour()
{
	if (WeaponSLot.IsValidIndex(3))
	{
		if (WeaponSLot[3] != CurrentWeapon->WeaponName)
			InitWeapon(WeaponSLot[3]);
	}
}

// Sets default values
ANR_Character::ANR_Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//HightLight
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue = 2;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HealthComponent = CreateDefaultSubobject<UNR_HealthComponent>(TEXT("CharHealthComponent"));
	if (HealthComponent)
	{
		HealthComponent->OnDead.AddDynamic(this, &ANR_Character::CharDead);
	}

	TokenComponent = CreateDefaultSubobject<UNR_TokenComponent>(TEXT("CharTokenComponent"));
	if (TokenComponent)
	{
		TokenComponent->InitTokens();
	}
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void ANR_Character::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		if (CursorMaterial && (GetLocalRole() == ROLE_AutonomousProxy || GetLocalRole() == ROLE_Authority))
		{
			CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
		}
	}

	//Sending initial stats to GameState for managing any changes and bind to Stats Update
	auto GameState = Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->OnTokensChanged.AddDynamic(this, &ANR_Character::UpdateStats);
		GameState->OnStartChangeLevel.AddDynamic(this, &ANR_Character::SaveWeaponToInstance);
	}

	GetCharacterMovement()->MaxWalkSpeed = Stats.BaseSpeed;

	auto myPlayerState = Cast<ANR_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (myPlayerState)
	{
		myPlayerState->LoadCounters();
	}

	//Init weapon
	auto myGameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (myGameInstance)
	{
		myGameInstance->LoadGame();
		WeaponSLot = myGameInstance->Weapons;
	}
	InitWeapon(WeaponSLot[0]);

	//Initialize character stats from GameInstance
	ApplyParamsOnStats();

	auto PlayerController = Cast<ANR_PlayerController>(GetController());
	PlayerController->OnEndGame.AddDynamic(this, &ANR_Character::AbsolutelyDead);
}

float ANR_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(!Stats.Immortality)
		HealthComponent->ChangeHealthValue(-DamageAmount);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//if (Cast<TSubclassOf<ANR_Projectile>>(DamageCauser))
	//{
	//	OnBossCausedDamage.Broadcast();
	//}
}

// Called every frame
void ANR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentCursor)
	{
		ANR_PlayerController* PlayerController = Cast<ANR_PlayerController>(GetController());

		FHitResult TraceHitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
		FVector CursorLocation = TraceHitResult.ImpactNormal;
		FRotator CursorRotation = CursorLocation.Rotation();

		CurrentCursor->SetWorldLocation(TraceHitResult.Location);
		CurrentCursor->SetWorldRotation(CursorRotation);
	}

	MovementTick(DeltaTime);
}

// Called to bind functionality to input
void ANR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ANR_Character::InputAxisX);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ANR_Character::InputAxisY);

	PlayerInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Pressed, this, &ANR_Character::InputAttackPressed);
	PlayerInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Released, this, &ANR_Character::InputAttackReleased);

	PlayerInputComponent->BindAction(TEXT("RollEvent"), EInputEvent::IE_Pressed, this, &ANR_Character::InputRollPressed);

	PlayerInputComponent->BindAction(TEXT("SwitchToWeapon1"), EInputEvent::IE_Pressed, this, &ANR_Character::SwitchToWeaponOne);
	PlayerInputComponent->BindAction(TEXT("SwitchToWeapon2"), EInputEvent::IE_Pressed, this, &ANR_Character::SwitchToWeaponTwo);
	PlayerInputComponent->BindAction(TEXT("SwitchToWeapon3"), EInputEvent::IE_Pressed, this, &ANR_Character::SwitchToWeaponThree);
	PlayerInputComponent->BindAction(TEXT("SwitchToWeapon4"), EInputEvent::IE_Pressed, this, &ANR_Character::SwitchToWeaponFour);
}

void ANR_Character::InputAxisX(float Value)
{
	AxisX = Value;
}

void ANR_Character::InputAxisY(float Value)
{
	AxisY = Value;
}

void ANR_Character::InputAttackPressed()
{
	if (HealthComponent && HealthComponent->GetIsAlive() && !RollEnable)
	{
		if(CurrentWeapon->WeaponSetting.AnimWeaponInfo.CharacterFireAnimMontage)
			PlayAnimMontage(CurrentWeapon->WeaponSetting.AnimWeaponInfo.CharacterFireAnimMontage);
		AttackEvent(true);
	}
}

void ANR_Character::InputAttackReleased()
{
	AttackEvent(false);
}

void ANR_Character::InputRollPressed()
{
	if (!RollEnable && bIsRollReady)
	{
		RollEnable = true;
		PlayAnimMontage(RollMontage);
		Stats.Immortality = true;

		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * 2;

		GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel5);
	}
}

ANR_Weapon* ANR_Character::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void ANR_Character::InitWeapon(FName IdWeaponName)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	if(ChangeWeaponSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ChangeWeaponSound, GetActorLocation());

	UNR_GameInstance* GameInstance = Cast<UNR_GameInstance>(GetGameInstance());
	FWeaponInfo WeaponInfo;

	if (GameInstance)
	{
		if (GameInstance->GetWeaponInfoByName(IdWeaponName, WeaponInfo))
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

					Weapon->Pawn = this;
					Weapon->WeaponSetting = WeaponInfo;
					Weapon->WeaponName = IdWeaponName;

					Weapon->OnWeaponFireStart.AddDynamic(this, &ANR_Character::WeaponFireStart);
					OnWeaponSwitched.Broadcast(IdWeaponName);
				}
			}
		}
	}
}


void ANR_Character::WeaponFireStart(UAnimMontage* Anim)
{
	PlayAnimMontage(Anim, 1.0f, "Mesh");

	WeaponFireStart_BP(Anim);
}

void ANR_Character::AddWeaponToInventory(FName WeaponName)
{
	WeaponSLot.Add(WeaponName);
}

void ANR_Character::MovementTick(float DeltaTime)
{
	ANR_PlayerController* PlayerController = Cast<ANR_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	PlayerController->DeprojectMousePositionToWorld(MouseLocation, LineEnd);

	LineEnd = (LineEnd * 5000) + MouseLocation;
	const FVector PointUnderCursor = FMath::LinePlaneIntersection(MouseLocation, LineEnd, GetActorLocation(), FVector(0, 0, 1));

	const float RotatingToCursor = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PointUnderCursor).Yaw;

	if (RollEnable && bIsRollReady)
	{
		if (RollTime < MaxRollTime)
		{
			AddMovementInput(GetActorForwardVector());
			RollTime = RollTime + DeltaTime;
		}
		else
		{
			RollEnable = false;
			RollTime = 0.0f;
			bIsRollReady = false;
			Stats.Immortality = false;

			GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed / 2;

			GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);

			OnRollReloadStart.Broadcast();
			GetWorldTimerManager().SetTimer(RollReloadHandle, this, &ANR_Character::RollReload, 3.0f, false, 3.0f);
		}
	}
	else
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

		SetActorRotation(FQuat(FRotator(0.0f, RotatingToCursor, 0.0f)));
	}
}

void ANR_Character::AttackEvent(bool bIsFire)
{
	if (GetCurrentWeapon())
	{
		GetCurrentWeapon()->FireButtonPressed(bIsFire);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ASnowBall_TPSCharacter::AttackCharEvent - CurrentWeapon - NULL"));
}

void ANR_Character::CharDead()
{
	auto PlayerController = Cast<ANR_PlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->CharacterDead();
	}
}

void ANR_Character::AbsolutelyDead(bool IsWin)
{
	PlayAnimMontage(DeathMontage);
}

void ANR_Character::TakeBonus(FName BonusType)
{
	auto MyPlayerState = Cast<ANR_PlayerState>(GetPlayerState());
	if (MyPlayerState)
	{
		MyPlayerState->IncrementNumBonus();
	}

	if (BonusType == "Aid")
	{
		HealthComponent->ChangeHealthValue(AidBonusValue);
	}
	else
	{
		if (BonusType == "Fire")
		{
			//Clean the timer before use
			if (GetWorldTimerManager().IsTimerActive(FinishFireBonusTimerHamdle))
				GetWorldTimerManager().ClearTimer(FinishFireBonusTimerHamdle);
			else
			{
				//Change params
				Stats.CoefFireSpeed = Stats.CoefFireSpeed / DamageBonusValue;
				Stats.CoefDamage = Stats.CoefDamage * DamageBonusValue;

				//Say weapon about changes
				OnWeaponParamsChange.Broadcast(Stats.CoefFireSpeed, Stats.CoefDamage);
			}

			//Set timer
			GetWorldTimerManager().SetTimer(FinishFireBonusTimerHamdle, this, &ANR_Character::FinishFireBonus, 5.0f, false, 5.0f);
		}
		else
		{
			if (BonusType == "Speed")
			{
				//Clean the timer before use
				if (GetWorldTimerManager().IsTimerActive(FinishMovementSpeedBonusTimerHamdle))
					GetWorldTimerManager().ClearTimer(FinishMovementSpeedBonusTimerHamdle);
				else
				{
					//Change params
					GetCharacterMovement()->MaxWalkSpeed = Stats.BaseSpeed + Stats.BaseSpeed * Stats.CoefMovementSpeed;
				}

				//Set timer
				GetWorldTimerManager().SetTimer(FinishMovementSpeedBonusTimerHamdle, this, &ANR_Character::FinishMovementSpeedBonus, 10.0f, false, 10.0f);
			}
			else
			{
				if (BonusType == "Immortality")
				{
					//Clean the timer before use
					if (GetWorldTimerManager().IsTimerActive(FinishImmortalityBonusTimerHamdle))
						GetWorldTimerManager().ClearTimer(FinishImmortalityBonusTimerHamdle);
					else
					{
						//Change params
						Stats.Immortality = true;
					}

					//Set timer
					GetWorldTimerManager().SetTimer(FinishImmortalityBonusTimerHamdle, this, &ANR_Character::FinishImmortalityBonus, ImmortalityBonusValue, false, ImmortalityBonusValue);

				}
				else
				{
					if (BonusType == "Freeze")
					{
						//Freeze function
						FreezeBonusFunction();
					}
					else
					{
						if (BonusType == "Bomb")
							BombDamageBonusFunction();
					}
				}
			}
		}
	}
}

void ANR_Character::FinishFireBonus()
{
	//Reset params
	Stats.CoefFireSpeed = Stats.CoefFireSpeed * DamageBonusValue;
	Stats.CoefDamage = Stats.CoefDamage / DamageBonusValue;

	//Say weapon about changes
	OnWeaponParamsChange.Broadcast(Stats.CoefFireSpeed, Stats.CoefDamage);
}

void ANR_Character::FinishImmortalityBonus()
{
	//Reset params
	Stats.Immortality = false;
}

void ANR_Character::FinishMovementSpeedBonus()
{
	//Reset params
	GetCharacterMovement()->MaxWalkSpeed -= Stats.BaseSpeed * Stats.CoefMovementSpeed;
}

void ANR_Character::FreezeBonusFunction()
{
	TArray<FHitResult> Hit;
	TArray<AActor*> Actors;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(),
		FreezeBonusRadius, ETraceTypeQuery::TraceTypeQuery3, false, Actors,
		EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Green, 
		FLinearColor::Red, 5.0f);

	for (int i = 0; i<Hit.Num(); i++)
	{
		INR_FreezeInterface* FreezeInterface = Cast<INR_FreezeInterface>(Hit[i].GetActor());
		if(FreezeInterface)
			FreezeInterface->Freeze(FreezeBonusValue);
	}
}

void ANR_Character::BombDamageBonusFunction()
{
	TArray<FHitResult> Hit;
	TArray<AActor*> Actors {this};

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(),
		BombDamageBonusRadius, ETraceTypeQuery::TraceTypeQuery3, false, Actors,
		EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Green,
		FLinearColor::Red, 5.0f);

	for (int i = 0; i < Hit.Num(); i++)
	{
		UGameplayStatics::ApplyPointDamage(Hit[i].GetActor(), BombDamageBonusValue, Hit[i].TraceStart, Hit[i], GetInstigatorController(), this, NULL);
	}
}

void ANR_Character::AddNewWeaponToWeaponSlot(FName NewWeapon)
{
	WeaponSLot.Add(NewWeapon);
}

TArray<FName> ANR_Character::GetWeaponSlot()
{
	return WeaponSLot;
}

void ANR_Character::TakeToken(FName Token)
{
	if (Token == "MoveSpeed")
	{
		TokenComponent->CollectToken(0);
	}
	if (Token == "Strength")
	{
		TokenComponent->CollectToken(1);
	}
	if (Token == "Health")
	{
		TokenComponent->CollectToken(2);
	}
	if (Token == "FireSpeed")
	{
		TokenComponent->CollectToken(3);
	}
	if (Token == "Power")
	{
		TokenComponent->CollectToken(4);
	}
	if (Token == "Luck")
	{
		TokenComponent->CollectToken(5);
	}
}

void ANR_Character::UpdateStats(TArray<int> Tokens)
{

	auto GameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto Table = GameInstance->TokensInfoTable;

	TArray<FName> RowNames = Table->GetRowNames();
	TArray<float> Multipliers;

	for (auto Name : RowNames)
	{
		auto Row = Table->FindRow<FTokensInfo>(Name, "");
		Multipliers.Add(Row->CharMultiplier);

	}

	Stats.CoefMovementSpeed += Tokens[0] * Multipliers[0];
	Stats.CoefDamage += Tokens[1] * Multipliers[1];
	Stats.CoefDamageResist += Tokens[2] * Multipliers[2];
	Stats.CoefFireSpeed += Tokens[3] * Multipliers[3];

	if (Tokens[4])
	{
		Stats.CoefMovementSpeed += Tokens[4] * Multipliers[4];
		Stats.CoefDamage += Tokens[4] * Multipliers[4];
		Stats.CoefDamageResist += Tokens[4] * Multipliers[4];
		Stats.CoefFireSpeed += Tokens[4] * Multipliers[4];
	}

	HealthComponent->CoefDamageResist = Stats.CoefDamageResist;
	GetCharacterMovement()->MaxWalkSpeed = Stats.BaseSpeed * Stats.CoefMovementSpeed;
	OnWeaponParamsChange.Broadcast(Stats.CoefFireSpeed, Stats.CoefDamage);

}

void ANR_Character::ApplyParamsOnStats()
{
	auto MyGameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	auto Achievements = MyGameInstance->GetAchievements();
	auto Tokens = MyGameInstance->GetTokens();

	TArray<float> Multipliers;

	auto GameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto Table = GameInstance->AchievementsInfoTable;

	TArray<FName> RowNames = Table->GetRowNames();

	for (int i = 0; i < 9; i++)
	{
		auto Row = Table->FindRow<FAchivementsInfo>(RowNames[i], "");
		auto Levels = Row->Levels;

		Multipliers.Add(Levels[Achievements[i]].Multiplier);
	}


	Stats.CoefDamage *= (1 + Multipliers[0] + Multipliers[2] + Multipliers[5] + Multipliers[7]);
	Stats.CoefFireSpeed *= (1 + Multipliers[3] + Multipliers[5] + Multipliers[7]);
	Stats.CoefDamageResist *= (1 + Multipliers[4] + Multipliers[5] + Multipliers[7]);
	Stats.CoefMovementSpeed *= (1 + Multipliers[6] + Multipliers[5] + Multipliers[7]);

	UpdateStats(Tokens);
}

TArray<float> ANR_Character::GetStats()
{
	TArray<float> Buff;
	Buff.Add(Stats.CoefDamage);
	Buff.Add(Stats.CoefFireSpeed);
	Buff.Add(Stats.CoefDamageResist);
	Buff.Add(Stats.CoefMovementSpeed);

	return Buff;
}

void ANR_Character::SaveWeaponToInstance()
{
	auto myGameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	myGameInstance->Weapons = WeaponSLot;
}

void ANR_Character::CharDead_BP_Implementation()
{
}


void ANR_Character::WeaponFireStart_BP_Implementation(UAnimMontage* Anim)
{
}

