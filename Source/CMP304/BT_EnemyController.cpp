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
	
	PawnSensing->OnSeePawn.AddDynamic(this, &ABT_EnemyController::OnSeePawn);
	
	RunBehaviorTree(BehaviorTree);

}

void ABT_EnemyController::OnSeePawn(APawn* PlayerPawn)
{
	ACMP304Character* Player = Cast<ACMP304Character>(PlayerPawn);

	if (Player)
	{
		SetCanSeePlayer(true, Player);
		RunRetriggerableTimer();
	}
}

void ABT_EnemyController::SetCanSeePlayer(bool SeePlayer, UObject* Player)
{
	if (SeePlayer)
	{
		GetBlackboardComponent()
			->SetValueAsBool(FName("Can See Player"), SeePlayer);

		GetBlackboardComponent()
			->SetValueAsObject(FName("Player Target"), Player);
	}
	else
	{
		GetBlackboardComponent()
			->SetValueAsBool(FName("Can See Player"), SeePlayer);

		ACharacter* EnemyChar = Cast<ACharacter>(GetPawn());
		EnemyChar->GetMesh()->GetAnimInstance()->StopAllMontages(0);
	}
}

void ABT_EnemyController::RunRetriggerableTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

	FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"),
		false, GetPawn());

	GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle,
		FunctionDelegate, PawnSensing->SensingInterval + 0.1f, false);
}
