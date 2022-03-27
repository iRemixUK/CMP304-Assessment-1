// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "CMP304Character.h"

ABT_EnemyController::ABT_EnemyController()
{
	// Pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void ABT_EnemyController::BeginPlay()
{
	Super::BeginPlay();
	
	// Runs behaviour tree and sensing component
	RunTreeAndSensing();
}

void ABT_EnemyController::RunTreeAndSensing()
{
	// If pawn sensing sees a pawn run player spotted function
	PawnSensing->OnSeePawn.AddDynamic(this, &ABT_EnemyController::PlayerSpotted);

	// Run behaviour tree
	RunBehaviorTree(BehaviourTree);
}

void ABT_EnemyController::PlayerSpotted(APawn* PlayerPawn)
{
	ACMP304Character* Player = Cast<ACMP304Character>(PlayerPawn);

	if (Player)
	{
		// Sets player spotted to true
		SetPlayerSpotted(true, Player);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Spotted"));

		CantSeePlayer();

		// Checks if player is in attack range
		AttackPlayer(Player->GetActorLocation());
	}
}

void ABT_EnemyController::SetPlayerSpotted(bool PlayerSpotted, UObject* Player)
{
	if (PlayerSpotted == true)
	{
		// Sets spotted to true in blackboard if player has been detected. Also sets player target so guard can chase player.
		GetBlackboardComponent()->SetValueAsBool(FName("Player Spotted"), PlayerSpotted);
		GetBlackboardComponent()->SetValueAsObject(FName("Player Target"), Player);
	}
	else
	{
		// Sets spotted to false in blackboard if player has not been detected
		GetBlackboardComponent()->SetValueAsBool(FName("Player Spotted"), PlayerSpotted);
	}
}

void ABT_EnemyController::CantSeePlayer()
{
	// Sets player spotted to false after two pawn sensing intervals
	// this means that if the player has not been spotted after 2 cycles
	// the guard has lost sight of the player
	GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

	FunctionDelegate.BindUFunction(this, FName("SetPlayerSpotted"), false, GetPawn());

	GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * 2.f, false);
}

void ABT_EnemyController::AttackPlayer(FVector PlayerLocation)
{
	FVector Distance = PlayerLocation - GetPawn()->GetActorLocation();

	// Check if guard is within 200 units of player, if so attack them. 
	if (Distance.Size() <= 200.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attacked"));
	}
}
