// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "Characters/SNonPlayerCharacter.h"
#include "Controllers/SAIController.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		ASNonPlayerCharacter* NonPlayerCharacter = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
		if (IsValid(NonPlayerCharacter))
		{
			if (!NonPlayerCharacter->IsNowAttacking())
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		ASNonPlayerCharacter* NonPlayerCharacter = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
		if (IsValid(NonPlayerCharacter))
		{
			NonPlayerCharacter->Attack();
		}
	}

	return EBTNodeResult::InProgress;
}
