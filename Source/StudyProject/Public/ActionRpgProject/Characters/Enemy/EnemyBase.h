// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRpgProject/Characters/BaseCharacter.h"
#include "ActionRpgProject/Characters/CharacterTypes.h"
#include "EnemyBase.generated.h"

UCLASS()
class STUDYPROJECT_API AEnemyBase : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;

	void SetEnemyState(EEnemyState InState);

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Die() override;

	UFUNCTION()
	double GetTargetDistance() const;

	UFUNCTION()
	bool IsTargetInRange(AActor* InTarget, double Radius);
	
	UFUNCTION()
	void MoveToTarget(AActor* InTarget);

	TObjectPtr<class AActor> ChoosePatrolTarget();

	UFUNCTION()
	void Attack() override;

	virtual bool CanAttack() override;

	virtual void HandleDamage(float DamageAmount) override;
	
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	
private:
	void PatrolTimerFinished();
	void CheckCombatTarget();
	void CheckPatrolTarget();

	/* AI Behavior */
	void InitializeEnemy();
	void ShowHealthBarToggle(bool bShow = false);
	void SetEnemyInterest(AActor* InActor);
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();

	/*Combat*/
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();

	void SpawnDefaultWeapon();
	void SpawnTreasure();
	
protected:
	UPROPERTY(EditAnywhere, Category="Combat")
	float DeathLifeSpan = 8.f;
	
private:
	//Components
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UHealthBarComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASwordWeapon> WeaponClass;
	
	//Navigation
	UPROPERTY()
	TObjectPtr<class AAIController> AIController;

	/* Combat Variables */
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category="Combat")
	float AttackMin = 0.2f;

	UPROPERTY(EditAnywhere, Category="Combat")
	float AttackMax = 0.5f;

	UPROPERTY(EditAnywhere, Category= "Combat")
	float PatrolSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category= "Combat")
	float ChasingSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	EEnemyState EnemyState = EEnemyState::EES_None;
	
	/* AI Behavior */
	UPROPERTY(EditInstanceOnly, Category="AI", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI")
	TArray<TObjectPtr<class AActor>> PatrolTargets;
	
	UPROPERTY(EditAnywhere, Category="AI")
	double CombatRadius = 800.0;

	UPROPERTY(EditAnywhere, Category="AI")
	double AttackRadius = 135.f;

	UPROPERTY(EditAnywhere, Category="AI")
	double AcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere, Category="AI")
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category="AI")
	float PatrolWaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category="AI")
	float PatrolWaitMax = 10.f;

	/* Rewards */
	UPROPERTY(EditAnywhere, Category="Rewards")
	TSubclassOf<class ATreasure> TreasureClass;
};
