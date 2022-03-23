// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class CMP304_API UTask_GetRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
private:

	class UNavigationSystemV1* Navigation;

	FVector RandomLocation;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

};
