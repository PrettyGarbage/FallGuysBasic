// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class STUDYPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyAIController();

	void BeginAI(APawn* InPawn);

	void EndAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void OnPatrolTimerElapsed();
	
public:
	FTimerHandle PatrolTimerHandle = FTimerHandle();

	float PatrolRepeatInterval = 3.f;

	static const float PatrolRadius;

	static const FName StartPatrolPositionKey;

	static const FName EndPatrolPositionKey;

	static const FName TargetActorKey;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI Controller", Meta=(AllowPrivateAccess))
	TObjectPtr<class UBlackboardData> BlackboardDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI Controller", Meta=(AllowPrivateAccess))
	TObjectPtr<class UBehaviorTree> BehaviorTree;
};
