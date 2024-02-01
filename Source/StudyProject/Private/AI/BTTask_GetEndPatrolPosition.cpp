// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_GetEndPatrolPosition.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/SNonPlayerCharacter.h"
#include "Controllers/SAIController.h"

UBTTask_GetEndPatrolPosition::UBTTask_GetEndPatrolPosition()
{
	NodeName = TEXT("GetEndPatrolPosition");
}

EBTNodeResult::Type UBTTask_GetEndPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if(Result == EBTNodeResult::Failed)
	{
		return Result;
	}

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if(!IsValid(AIController))
	{
		return Result = EBTNodeResult::Failed;
	}

	ASNonPlayerCharacter* NonPlayerCharacter = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
	if(!IsValid(NonPlayerCharacter))
	{
		return Result = EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(NonPlayerCharacter->GetWorld());
	if(!IsValid(NavSystem))
	{
		return Result = EBTNodeResult::Failed;
	}

	FVector StartPatrolPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ASAIController::StartPatrolPositionKey);
	FNavLocation EndPatrolPosition;
	if(NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, AIController->PatrolRadius, EndPatrolPosition))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASAIController::EndPatrolPositionKey, EndPatrolPosition.Location);
		return Result = EBTNodeResult::Succeeded;
	}

	return Result;
}


