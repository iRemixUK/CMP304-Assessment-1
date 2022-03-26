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
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void ABT_EnemyController::BeginPlay()
{
	Super::BeginPlay();
	RunTreeAndSensing();
}

void ABT_EnemyController::RunTreeAndSensing()
{
	PawnSensing->OnSeePawn.AddDynamic(this, &ABT_EnemyController::PlayerSpotted);
	RunBehaviorTree(BehaviourTree);
}

void ABT_EnemyController::PlayerSpotted(APawn* PlayerPawn)
{
	ACMP304Character* Player = Cast<ACMP304Character>(PlayerPawn);

	if (Player)
	{
		SetPlayerSpotted(true, Player);
		CantSeePlayer();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Spotted"));

		FVector PlayerLocation = Player->GetActorLocation();
		FVector Distance = PlayerLocation - GetPawn()->GetActorLocation();

		// Check if guard is within 200 units of player, if so attack them. 
		if (Distance.Size() <= 200.f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attacked"));
		}
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