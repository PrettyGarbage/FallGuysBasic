// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UBTTask_TurnToTargetActor : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TurnToTargetActor();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
