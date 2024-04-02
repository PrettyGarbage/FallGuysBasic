// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/AI/BTService_DetectActorByPawnSensing.h"
#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Characters/Enemy/ActionBossAICharacter.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UBTService_DetectActorByPawnSensing::UBTService_DetectActorByPawnSensing()
{
	NodeName = TEXT("DetectActorByPawnSensing");
	Interval = 1.f;
}

void UBTService_DetectActorByPawnSensing::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                   float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionBossAICharacter* ActionAICharacter = Cast<AActionBossAICharacter>(EnemyAIController->GetPawn());
		if(IsValid(ActionAICharacter))
		{
			ReleasePlayerCharacter(EnemyAIController, ActionAICharacter, OwnerComp);
		}
	}
}

void UBTService_DetectActorByPawnSensing::ReleasePlayerCharacter(AEnemyAIController* EnemyAIController, AActionBossAICharacter* ActionAICharacter, UBehaviorTreeComponent& OwnerComp)
{
	UWorld* World = ActionAICharacter->GetWorld();
	if(IsValid(World))
	{
		FVector CenterPosition = ActionAICharacter->GetActorLocation();
		float DetectRadius = ActionAICharacter->DetectRange;
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams  CollisionQueryParams(NAME_None, false, ActionAICharacter);
		bool bResult = World->OverlapMultiByChannel(OverlapResults, CenterPosition, FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel12, FCollisionShape::MakeSphere(DetectRadius),
			CollisionQueryParams);

		if(bResult)
		{
			bool bIsExistPlayerCharacter = false;
			for(auto const& OverlapResult : OverlapResults)
			{
				AActionCharacter* PlayerCharacter = Cast<AActionCharacter>(OverlapResult.GetActor());
				if(IsValid(PlayerCharacter))
				{
					bIsExistPlayerCharacter = true;
				}
				//DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Cyan, false, 0.5f);
			}

			if(!bIsExistPlayerCharacter)
			{
				OwnerComp.GetBlackboardComponent()->ClearValue(EnemyAIController->TargetActorKey);
			}
		}
	}
}
