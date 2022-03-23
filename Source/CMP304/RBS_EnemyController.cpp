// Fill out your copyright notice in the Description page of Project Settings.


#include "RBS_EnemyController.h"
#include "NavigationSystem.h"

void ARBS_EnemyController::BeginPlay()
{
    Super::BeginPlay();

    Navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    Patrol();
}

void ARBS_EnemyController::Patrol()
{
    if (Navigation)
    {
        Navigation->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(),
            RandomLocation, 10000.0f);

        MoveToLocation(RandomLocation);
    }
}
