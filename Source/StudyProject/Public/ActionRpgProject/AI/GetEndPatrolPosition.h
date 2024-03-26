// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetEndPatrolPosition.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UGetEndPatrolPosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UGetEndPatrolPosition();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
