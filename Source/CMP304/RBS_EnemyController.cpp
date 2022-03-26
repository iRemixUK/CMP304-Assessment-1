// Fill out your copyright notice in the Description page of Project Settings.


#include "RBS_EnemyController.h"
#include "NavigationSystem.h"

void ARBS_EnemyController::BeginPlay()
{
    Super::BeginPlay();

    // Get navigation system
    Navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    Patrol();
}

void ARBS_EnemyController::Patrol()
{
    if (Navigation)
    {
        if (GetPawn())
        {
            // Gets random location available in the navigation area and AI moves to it
            Navigation->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 10000.0f);
            MoveToLocation(RandomLocation);
        }
    }
}
