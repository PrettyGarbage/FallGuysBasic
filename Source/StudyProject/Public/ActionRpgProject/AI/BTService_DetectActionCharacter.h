// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectActionCharacter.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UBTService_DetectActionCharacter : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectActionCharacter();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
