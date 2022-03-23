// Fill out your copyright notice in the Description page of Project Settings.
#include "Task_GetRandomLocation.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (Navigation)
	{
		// Choose a random location within the navigation area.
		Navigation->K2_GetRandomReachablePointInRadius(GetWorld(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), RandomLocation, 10000.0f);
	}

	else
	{
		return EBTNodeResult::Failed;
	}

	// inform blackboard about random location result
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("Random Patrol Location"), RandomLocation);
	return EBTNodeResult::Succeeded;
}
