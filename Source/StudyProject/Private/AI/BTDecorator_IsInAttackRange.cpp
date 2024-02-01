// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsInAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/SNonPlayerCharacter.h"
#include "Controllers/SAIController.h"

const float UBTDecorator_IsInAttackRange::AttackRange = 200.f;

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if(IsValid(AIController))
	{
		ASNonPlayerCharacter* NonPlayerCharacter = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
		if(IsValid(NonPlayerCharacter))
		{
			ASCharacter* TargetCharacter = Cast<ASCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASAIController::TargetActorKey));
			if(IsValid(TargetCharacter))
			{
				return bResult = NonPlayerCharacter->GetDistanceTo(TargetCharacter) <= AttackRange;
			}
		}
	}
	return bResult = false;
}
