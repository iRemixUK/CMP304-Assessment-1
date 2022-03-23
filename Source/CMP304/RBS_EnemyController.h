// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RBS_EnemyController.generated.h"

UCLASS()
class CMP304_API ARBS_EnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
    void BeginPlay() override;
    
    UFUNCTION(BlueprintCallable)
        void Patrol();

private:
    FVector RandomLocation;
    class UNavigationSystemV1* Navigation;
};
