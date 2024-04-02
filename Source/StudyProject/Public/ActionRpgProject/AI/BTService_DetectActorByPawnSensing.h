// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectActorByPawnSensing.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UBTService_DetectActorByPawnSensing : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectActorByPawnSensing();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void ReleasePlayerCharacter(class AEnemyAIController* EnemyAIController, class AActionBossAICharacter* ActionAICharacter, class UBehaviorTreeComponent& OwnerComp);
};
