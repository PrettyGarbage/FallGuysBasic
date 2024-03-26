// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Controller/EnemyAIController.h"

#include "NavigationSystem.h"
#include "ActionRpgProject/Define/DefineVariables.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

const float AEnemyAIController::PatrolRadius = 500.0f;
const FName AEnemyAIController::StartPatrolPositionKey = GStartPatrolPositionKey;
const FName AEnemyAIController::EndPatrolPositionKey = GEndPatrolPositionKey;
const FName AEnemyAIController::TargetActorKey = GTargetActorKey;

// Sets default values
AEnemyAIController::AEnemyAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
}

void AEnemyAIController::BeginAI(APawn* InPawn)
{
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if(IsValid(BlackboardComponent) && UseBlackboard(BlackboardDataAsset, BlackboardComponent))
	{
		bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
		ensure(bRunSucceeded);
		
		BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());
	}
}

void AEnemyAIController::EndAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(IsValid(BehaviorTreeComponent))
	{
		BehaviorTreeComponent->StopTree();
	}
}

// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* ControlledPawn = GetPawn();
	if(IsValid(ControlledPawn))
	{
		BeginAI(ControlledPawn);
	}
}

void AEnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	EndAI();
}

void AEnemyAIController::OnPatrolTimerElapsed()
{
	APawn* ControlledPawn = GetPawn();
	if(IsValid(ControlledPawn))
	{
		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if(IsValid(NavSystem))
		{
			FNavLocation NextLocation;
			if(NavSystem->GetRandomPointInNavigableRadius(ControlledPawn->GetActorLocation(), PatrolRadius, NextLocation))
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
			}
		}
	}
}

