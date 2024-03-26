// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/AI/BTTask_EnemyAttack.h"

#include "ActionRpgProject/Characters/Enemy/ActionAICharacter.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_EnemyAttack::UBTTask_EnemyAttack()
{
	bNotifyTick = true;
}

void UBTTask_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionAICharacter* EnemyCharacter = Cast<AActionAICharacter>(EnemyAIController->GetPawn());
		if(IsValid(EnemyCharacter) && !EnemyCharacter->IsNowAttacking())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionAICharacter* EnemyCharacter = Cast<AActionAICharacter>(EnemyAIController->GetPawn());
		if(IsValid(EnemyCharacter))
		{
			EnemyCharacter->CallAttackLogic();
			UKismetSystemLibrary::PrintString(GetWorld(), "Attack");
		}
	}

	return EBTNodeResult::InProgress;
}
