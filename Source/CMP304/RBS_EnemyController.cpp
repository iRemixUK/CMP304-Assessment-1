// Fill out your copyright notice in the Description page of Project Settings.


#include "RBS_EnemyController.h"
#include "NavigationSystem.h"

void ARBS_EnemyController::BeginPlay()
{
    Super::BeginPlay();

    NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    RandomPatrol();
}

void ARBS_EnemyController::RandomPatrol()
{
    if (NavArea)
    {
        NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(),
            RandomLocation, 15000.0f);

        MoveToLocation(RandomLocation);
    }
}
