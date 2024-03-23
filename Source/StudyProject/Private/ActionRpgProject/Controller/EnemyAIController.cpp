// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Controller/EnemyAIController.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


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
		
		//BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());
	}
}

void AEnemyAIController::EndAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(IsValid(BehaviorTreeComponent))
	{
		BehaviorTreeComponent->StopTree();
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Stop Behaviour Tree"));
	}
}

// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &AEnemyAIController::OnPatrolTimerElapsed, PatrolRepeatInterval, true);
}

void AEnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);
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

