// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "rbs_Enemy.generated.h"

UENUM(BlueprintType)
enum class ECharState : uint8
{
	Patrol		UMETA(DisplayName = "Patrol"),
	Chase		UMETA(DisplayName = "Chase"),
	Attack		UMETA(DisplayName = "Attack")
};

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


public:

	bool PlayerDetected;
	bool CanAttackPlayer;

	UPROPERTY(BlueprintReadWrite)
		bool CanDealDamage;

	class ACMP304Character* PlayerREF;

	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerCollisionDetection;

	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerAttackCollisionDetection;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;

	class ARBS_EnemyController* AIController;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	UPROPERTY(EditAnywhere)
		float StoppingDistance = 100.0f;
	
	// 0 for patrol, 1 for chase, 2 for attack.
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int state = 0;

	FTimerHandle SeekPlayerTimerHandle;

	UFUNCTION()
		void MoveToPlayer();

	UFUNCTION()
		void SeekPlayer();

	UFUNCTION()
		void StopSeekingPlayer();

	UFUNCTION()
		void OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};