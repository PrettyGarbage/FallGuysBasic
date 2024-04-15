// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/AI/BTDecorator_IsInEnemyByBoss.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Characters/Enemy/ActionBossAICharacter.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

const float UBTDecorator_IsInEnemyByBoss::AttackRange = 2800.0f;

UBTDecorator_IsInEnemyByBoss::UBTDecorator_IsInEnemyByBoss()
{
	NodeName = TEXT("IsInEnemyByBoss");
}

bool UBTDecorator_IsInEnemyByBoss::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionBossAICharacter* BossAICharacter = Cast<AActionBossAICharacter>(EnemyAIController->GetPawn());
		if(IsValid(BossAICharacter))
		{
			AActionCharacter* TargetCharacter = Cast<AActionCharacter>(EnemyAIController->GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetActorKey));
			if(IsValid(TargetCharacter))
			{
				return bResult = (BossAICharacter->GetDistanceTo(TargetCharacter) <= AttackRange);
			}
		}
	}

	return bResult = false;
}
