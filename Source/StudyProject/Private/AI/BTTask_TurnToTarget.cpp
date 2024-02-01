// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/SNonPlayerCharacter.h"
#include "Controllers/SAIController.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTargetActor");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
		if (IsValid(NPC))
		{
			if (ASCharacter* TargetPlayer = Cast<ASCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIController->TargetActorKey)))
			{
				FVector LookVector = TargetPlayer->GetActorLocation() - NPC->GetActorLocation();
				LookVector.Z = 0.f;
				FRotator TargetRotation = FRotationMatrix::MakeFromX(LookVector).Rotator();
				NPC->SetActorRotation(FMath::RInterpTo(NPC->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 2.f));

				return Result = EBTNodeResult::Succeeded;
			}
		}
	}

	return Result = EBTNodeResult::Failed;
}