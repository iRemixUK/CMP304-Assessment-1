// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "rbs_Enemy.generated.h"

UCLASS()
class CMP304_API Arbs_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Arbs_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Functions
	UFUNCTION()
		void MoveToPlayer();

	UFUNCTION()
		void FindPlayer();

	UFUNCTION()
		void StopFindingPlayer();

	UFUNCTION()
		void PlayerSpottedOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void PlayerSpottedOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void AttackRangeOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void AttackRangeOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void PlayerSpottedOverlap();

	UFUNCTION()
		void AttackOverlap();

	// Variables 
	bool PlayerSpotted;
	bool InAttackRange;

	class ACMP304Character* Player;
	class ARBS_EnemyController* AIController;

	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerDetection;

	UPROPERTY(EditAnywhere)
		class USphereComponent* AttackDetection;

	void AIMoveFinished(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	UPROPERTY(EditAnywhere)
		float StoppingDistance = 100.0f;

	FTimerHandle TimerHandle;
};
