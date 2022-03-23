#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BT_EnemyController.generated.h"


UCLASS()
class CMP304_API ABT_EnemyController : public AAIController
{
	GENERATED_BODY()

	ABT_EnemyController();

public:
	void BeginPlay() override;

	UFUNCTION()
		void PlayerSpotted(APawn* PlayerPawn);

	UFUNCTION()
		void SetPlayerSpotted(bool PlayerSpotted, class UObject* Player);

	UFUNCTION()
	void CantSeePlayer();

	FTimerHandle RetriggerableTimerHandle;
	FTimerDelegate FunctionDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviourTree;

	UPROPERTY(EditAnywhere)
		class UPawnSensingComponent* PawnSensing;

};