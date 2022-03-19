// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FSM_EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API AFSM_EnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
    void BeginPlay() override;

private:

    class UNavigationSystemV1* NavArea;

    FVector RandomLocation;

public:

    UFUNCTION()
        void RandomPatrol();
};
