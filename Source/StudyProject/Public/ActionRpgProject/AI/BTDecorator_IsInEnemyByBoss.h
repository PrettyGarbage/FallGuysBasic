// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInEnemyByBoss.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UBTDecorator_IsInEnemyByBoss : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsInEnemyByBoss();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	static const float AttackRange;
};
