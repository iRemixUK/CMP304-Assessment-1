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

	PlayerCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));

	PlayerCollisionDetection->SetupAttachment(RootComponent);

	PlayerAttackCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));

	PlayerAttackCollisionDetection->SetupAttachment(RootComponent);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
}

// Called when the game starts or when spawned
void Arbs_Enemy::BeginPlay()
{
		Super::BeginPlay();

		AIController = Cast<ARBS_EnemyController>(GetController());

		// Patrol state
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject
			(this, &Arbs_Enemy::OnAIMoveCompleted);

		// Detects player 
		PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
			&Arbs_Enemy::OnPlayerDetectedOverlapBegin);
	
		PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
			&Arbs_Enemy::OnPlayerDetectedOverlapEnd);

		PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
			&Arbs_Enemy::OnPlayerAttackOverlapBegin);

		PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
			&Arbs_Enemy::OnPlayerAttackOverlapEnd);

		DamageCollision->OnComponentBeginOverlap.AddDynamic(this,
			&Arbs_Enemy::OnDealDamageOverlapBegin);
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

void Arbs_Enemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!PlayerDetected)
	{
		AIController->RandomPatrol();
	}
	else if (PlayerDetected && CanAttackPlayer)
	{
		StopSeekingPlayer();

		// attack player
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attacked"));
	}
}

void Arbs_Enemy::MoveToPlayer()
{
	AIController->MoveToLocation(PlayerREF->GetActorLocation(), StoppingDistance, true);
}

void Arbs_Enemy::SeekPlayer()
{
	MoveToPlayer();
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this,
		&Arbs_Enemy::SeekPlayer, 0.25f, true);
}

void Arbs_Enemy::StopSeekingPlayer()
{
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

// Detects player
void Arbs_Enemy::OnPlayerDetectedOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF)
	{
		PlayerDetected = true;
		SeekPlayer();

	}
}

void Arbs_Enemy::OnPlayerDetectedOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF)
	{
		state = 0;
		PlayerDetected = false;
		StopSeekingPlayer();
		AIController->RandomPatrol();
	}
}

void Arbs_Enemy::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF)
	{
		CanAttackPlayer = true;
		CanDealDamage = true;
		state = 2;
	}
}

void Arbs_Enemy::OnPlayerAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF)
	{
		CanAttackPlayer = false;

		SeekPlayer();

		state = 1;
	}
}

void Arbs_Enemy::OnDealDamageOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF && CanDealDamage)
	{
		// deal damage to player
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player damaged!"));
	}
}

