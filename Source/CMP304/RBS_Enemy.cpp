// Fill out your copyright notice in the Description page of Project Settings.


#include "rbs_Enemy.h"
#include "CMP304Character.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "RBS_EnemyController.h"

// Sets default values
Arbs_Enemy::Arbs_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetection =CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));

	PlayerDetection->SetupAttachment(RootComponent);

	AttackDetection =CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));

	AttackDetection->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void Arbs_Enemy::BeginPlay()
{
		Super::BeginPlay();

		// Gets AI controller
		AIController = Cast<ARBS_EnemyController>(GetController());

		// AIMoveFinished connected to on request finished event which lets us know when the ai movement has finished.
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &Arbs_Enemy::AIMoveFinished);

		// Spots player when in range
		PlayerDetection->OnComponentBeginOverlap.AddDynamic(this, &Arbs_Enemy::PlayerSpottedOverlapBegin);
	
		// Loses sight of player when no longer in range
		PlayerDetection->OnComponentEndOverlap.AddDynamic(this, &Arbs_Enemy::PlayerSpottedOverlapEnd);

		// Attacks player when in range
		AttackDetection->OnComponentBeginOverlap.AddDynamic(this, &Arbs_Enemy::AttackRangeOverlapBegin);

		// Stops attacking when no longer in range
		AttackDetection->OnComponentEndOverlap.AddDynamic(this, &Arbs_Enemy::AttackRangeOverlapEnd);
}

// Called every frame
void Arbs_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void Arbs_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void Arbs_Enemy::AIMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!PlayerSpotted)
	{
		// Player not spotted / no longer spotted so start patrolling again
		AIController->Patrol();
	}
	else if (PlayerSpotted && InAttackRange)
	{
		// Stops looking for player and attacks them
		StopFindingPlayer();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attacked"));
	}
}

void Arbs_Enemy::MoveToPlayer()
{
	// Travels to player's location
	AIController->MoveToLocation(Player->GetActorLocation(), StoppingDistance, true);
}

void Arbs_Enemy::FindPlayer()
{
	// Calls move to player function every 0.25 seconds
	// This ensures that the AI is moving to the updated position of the player 
	MoveToPlayer();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &Arbs_Enemy::FindPlayer, 0.25f, true);
}

void Arbs_Enemy::StopFindingPlayer()
{
	// Stops looking for player by clearing the retriggable timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

// Detects player
void Arbs_Enemy::PlayerSpottedOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Player has overlapped with enemy line of sight, so enemy has spotted the player
	Player = Cast<ACMP304Character>(OtherActor);

	if (Player)
	{
		PlayerSpotted = true;
		FindPlayer();
	}
}

void Arbs_Enemy::PlayerSpottedOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Player no longer in the enemy line of sight, so the player is no longer spotted and will go back to patrolling.
	Player = Cast<ACMP304Character>(OtherActor);

	if (Player)
	{
		PlayerSpotted = false;
		StopFindingPlayer();
		AIController->Patrol();
	}
}

void Arbs_Enemy::AttackRangeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Player is in attack range
	Player = Cast<ACMP304Character>(OtherActor);

	if (Player)
	{
		InAttackRange = true;
	}
}

void Arbs_Enemy::AttackRangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Player no longer in attack range
	Player = Cast<ACMP304Character>(OtherActor);

	if (Player)
	{
		InAttackRange = false;
		FindPlayer();
	}
}



