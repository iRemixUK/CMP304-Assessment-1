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
	}
}

void ABT_EnemyController::SetPlayerSpotted(bool PlayerSpotted, UObject* Player)
{
	if (PlayerSpotted)
	{
		GetBlackboardComponent()
			->SetValueAsBool(FName("Player Spotted"), PlayerSpotted);

		GetBlackboardComponent()
			->SetValueAsObject(FName("Player Target"), Player);
	}
	else
	{
		GetBlackboardComponent()
			->SetValueAsBool(FName("Player Spotted"), PlayerSpotted);
	}
}

void ABT_EnemyController::CantSeePlayer()
{
	// Sets player spotted to false every 0.1 seconds after the pawn sensing interval
	// This means that if the player has not been spotted 0.1 seconds after the interval
	// The guard has lost sight of the player
	GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

	FunctionDelegate.BindUFunction(this, FName("PlayerSpotted"),
		false, GetPawn());

	GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle,
		FunctionDelegate, PawnSensing->SensingInterval + 0.1f, false);
}
