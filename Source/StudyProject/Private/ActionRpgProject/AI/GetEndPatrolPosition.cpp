// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/AI/GetEndPatrolPosition.h"

#include "NavigationSystem.h"
#include "ActionRpgProject/Characters/Enemy/ActionAICharacter.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UGetEndPatrolPosition::UGetEndPatrolPosition()
{
	NodeName = TEXT("GetEndAIPatrolPosition");
}

EBTNodeResult::Type UGetEndPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if(Result == EBTNodeResult::Failed)
	{
		return Result;
	}
	
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(!IsValid(EnemyAIController))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("EnemyAIController is not valid"));
		return Result = EBTNodeResult::Failed;
	}

	AActionAICharacter* AICharacter = Cast<AActionAICharacter>(EnemyAIController->GetPawn());
	if(!IsValid(AICharacter))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("EnemyBase is not valid"));

		return Result = EBTNodeResult::Failed;
	}
	
	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(AICharacter->GetWorld());
	if(!IsValid(Nav))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Nav is not valid"));

		return Result = EBTNodeResult::Failed;
	}

	FVector StartPatrolPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AEnemyAIController::StartPatrolPositionKey);
	FNavLocation EndPatrolLocation;
	if(Nav->GetRandomPointInNavigableRadius(StartPatrolPosition, AEnemyAIController::PatrolRadius, EndPatrolLocation))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("StartPatrolPosition"));

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::EndPatrolPositionKey, EndPatrolLocation.Location);
		return Result = EBTNodeResult::Succeeded;
	}

	return Result;
}
