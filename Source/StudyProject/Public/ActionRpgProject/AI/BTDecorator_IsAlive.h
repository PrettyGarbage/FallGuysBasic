// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsAlive.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UBTDecorator_IsAlive : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsAlive();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
