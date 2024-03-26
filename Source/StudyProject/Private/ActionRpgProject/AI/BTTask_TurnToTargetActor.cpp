// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/AI/BTTask_TurnToTargetActor.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Characters/Enemy/ActionAICharacter.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTargetActor::UBTTask_TurnToTargetActor()
{
	NodeName = TEXT("TurnToTargetActor");
}

EBTNodeResult::Type UBTTask_TurnToTargetActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionAICharacter* EnemyCharacter = Cast<AActionAICharacter>(EnemyAIController->GetPawn());
		if(IsValid(EnemyAIController))
		{
			if(AActionCharacter* TargetCharacter = Cast<AActionCharacter>(EnemyAIController->GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetActorKey)))
			{
				FVector LookAtTarget = TargetCharacter->GetActorLocation();
				LookAtTarget.Z = EnemyCharacter->GetActorLocation().Z;
				EnemyCharacter->SetActorRotation((LookAtTarget - EnemyCharacter->GetActorLocation()).Rotation());
				Result = EBTNodeResult::Succeeded;
			}
		}
	}

	return Result = EBTNodeResult::Failed;
}
