// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/SAIController.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

const float ASAIController::PatrolRadius = 500.f;
const FName ASAIController::StartPatrolPositionKey = FName(TEXT("StartPatrolPosition"));
const FName ASAIController::EndPatrolPositionKey = FName(TEXT("EndPatrolPosition"));
const FName ASAIController::TargetActorKey = FName(TEXT("TargetActor"));

ASAIController::ASAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
}

void ASAIController::BeginAI(APawn* InPawn)
{
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if(IsValid(BlackboardComponent) && UseBlackboard(BlackboardDataAsset, BlackboardComponent))
	{
		bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
		ensure(bRunSucceeded);
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("RunBehaviorTree"));

		BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());
	}
}

void ASAIController::EndAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(IsValid(BehaviorTreeComponent))
	{
		BehaviorTreeComponent->StopTree();
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("StopTree"));
	}
}

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* ControlledPawn = GetPawn();
	if(IsValid(ControlledPawn))
	{
		BeginAI(ControlledPawn);
	}
}

void ASAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	EndAI();
}

void ASAIController::OnPatrolTimerElapsed()
{
	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn))
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (IsValid(NavSystem))
		{
			FNavLocation NextLocation;
			if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, PatrolRadius, NextLocation))
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
			}
		}
	}
}
