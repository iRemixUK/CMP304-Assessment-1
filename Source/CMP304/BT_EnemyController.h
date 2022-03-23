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
	UFUNCTION(BlueprintCallable)
	void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
		void RunTreeAndSensing();

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