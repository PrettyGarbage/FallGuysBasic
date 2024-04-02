// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/AI/BTDecorator_IsAlive.h"

#include "ActionRpgProject/Characters/Enemy/ActionBossAICharacter.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"

UBTDecorator_IsAlive::UBTDecorator_IsAlive()
{
	NodeName = TEXT("Is Alive");
}

bool UBTDecorator_IsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult =  Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionBossAICharacter* BossAICharacter = Cast<AActionBossAICharacter>(EnemyAIController->GetPawn());
		if(IsValid(BossAICharacter))
		{
			return bResult = BossAICharacter->GetEnemyState() != EEnemyState::EES_Dead;
		}
	}

	return bResult = false;
}


