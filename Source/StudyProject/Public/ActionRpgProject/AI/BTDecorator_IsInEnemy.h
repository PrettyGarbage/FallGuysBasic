// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInEnemy.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UBTDecorator_IsInEnemy : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_IsInEnemy();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	static const float AttackRange;
};
