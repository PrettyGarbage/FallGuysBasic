// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/AI/BTDecorator_IsInEnemy.h"

#include "AIController.h"
#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Characters/Enemy/ActionAICharacter.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

const float UBTDecorator_IsInEnemy::AttackRange = 200.f;

UBTDecorator_IsInEnemy::UBTDecorator_IsInEnemy()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionAICharacter* EnemyCharacter = Cast<AActionAICharacter>(EnemyAIController->GetPawn());
		if(IsValid(EnemyAIController))
		{
			AActionCharacter* TargetCharacter = Cast<AActionCharacter>(EnemyAIController->GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetActorKey));
			if(IsValid(TargetCharacter))
			{
				//DrawDebugSphere(GetWorld(), TargetCharacter->GetActorLocation(), AttackRange, 12, FColor::Red, false, 1.f, 0, 1.f);
				
				return bResult = (EnemyCharacter->GetDistanceTo(TargetCharacter) <= AttackRange);
			}
		}
	}

	return bResult = false;
}
