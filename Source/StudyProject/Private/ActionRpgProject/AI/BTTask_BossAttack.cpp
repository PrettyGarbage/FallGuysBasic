// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/AI/BTTask_BossAttack.h"

#include "ActionRpgProject/Characters/Enemy/ActionBossAICharacter.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_BossAttack::UBTTask_BossAttack()
{
	bNotifyTick = true;
}

void UBTTask_BossAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionBossAICharacter* BossAICharacter = Cast<AActionBossAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
		if(IsValid(BossAICharacter) && !BossAICharacter->IsNowAttacking())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionBossAICharacter* BossAICharacter = Cast<AActionBossAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
		if(IsValid(BossAICharacter))
		{
			if(BossAICharacter->GetEnemyState() == EEnemyState::EES_Dead)
			{
				return EBTNodeResult::Failed;
			}
			BossAICharacter->CallAttackLogic();
		}
	}

	return EBTNodeResult::InProgress;
}
