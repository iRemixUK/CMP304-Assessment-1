// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BT_EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API ABT_EnemyController : public AAIController
{
	GENERATED_BODY()

	ABT_EnemyController();
public:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviourTree;

	UPROPERTY(EditAnywhere)
		class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
		void PlayerSpotted(APawn* PlayerPawn);

	UFUNCTION() 
		void SetPlayerSpotted(bool PlayerSpotted, class UObject* Player);

	FTimerHandle RetriggerableTimerHandle;
	FTimerDelegate FunctionDelegate;

	void CantSeePlayer();
};