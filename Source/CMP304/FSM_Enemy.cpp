// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_Enemy.h"
#include "CMP304Character.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "FSM_EnemyController.h"

// Sets default values
AFSM_Enemy::AFSM_Enemy()
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
void AFSM_Enemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AFSM_EnemyController>(GetController());

	switch (state)
	{
		case ECharState(0):

		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject
		(this, &AFSM_Enemy::OnAIMoveCompleted);
		
		PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
			&AFSM_Enemy::OnPlayerDetectedOverlapBegin);

		PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
			&AFSM_Enemy::OnPlayerDetectedOverlapEnd);

		PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
			&AFSM_Enemy::OnPlayerAttackOverlapBegin);

		PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
			&AFSM_Enemy::OnPlayerAttackOverlapEnd);

		DamageCollision->OnComponentBeginOverlap.AddDynamic(this,
			&AFSM_Enemy::OnDealDamageOverlapBegin);
		break;

		default:
			break;
	}

}

// Called every frame
void AFSM_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFSM_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFSM_Enemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
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

void AFSM_Enemy::MoveToPlayer()
{
	AIController->MoveToLocation(PlayerREF->GetActorLocation(), StoppingDistance, true);
}

void AFSM_Enemy::SeekPlayer()
{
	MoveToPlayer();
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this,
		&AFSM_Enemy::SeekPlayer, 0.25f, true);
}

void AFSM_Enemy::StopSeekingPlayer()
{
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

void AFSM_Enemy::OnPlayerDetectedOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF)
	{
		PlayerDetected = true;
		SeekPlayer();
	}
}

void AFSM_Enemy::OnPlayerDetectedOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF)
	{
		PlayerDetected = false;
		StopSeekingPlayer();
		AIController->RandomPatrol();
	}
}

void AFSM_Enemy::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF)
	{
		CanAttackPlayer = true;
	}
}

void AFSM_Enemy::OnPlayerAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF)
	{
		CanAttackPlayer = false;

		SeekPlayer();
	}
}

void AFSM_Enemy::OnDealDamageOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<ACMP304Character>(OtherActor);
	if (PlayerREF && CanDealDamage)
	{
		// deal damage to player
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player damaged!"));
	}
}

