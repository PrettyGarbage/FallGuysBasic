// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/AI/BTService_DetectActionCharacter.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Characters/Enemy/ActionAICharacter.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UBTService_DetectActionCharacter::UBTService_DetectActionCharacter()
{
	NodeName = TEXT("DetectActionCharacter");
	Interval = 1.f;
}

void UBTService_DetectActionCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(IsValid(EnemyAIController))
	{
		AActionAICharacter* ActionAICharacter = Cast<AActionAICharacter>(EnemyAIController->GetPawn());
		if(IsValid(ActionAICharacter))
		{
			UWorld* World = ActionAICharacter->GetWorld();
			if(IsValid(World))
			{
				FVector CenterPosition = ActionAICharacter->GetActorLocation();
				float DetectRadius = 300.f;
				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams  CollisionQueryParams(NAME_None, false, ActionAICharacter);
				bool bResult = World->OverlapMultiByChannel(OverlapResults, CenterPosition, FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel12, FCollisionShape::MakeSphere(DetectRadius),
					CollisionQueryParams);

				if(bResult)
				{
					for(auto const& OverlapResult : OverlapResults)
					{
						AActionCharacter* PlayerCharacter = Cast<AActionCharacter>(OverlapResult.GetActor());
						if(IsValid(PlayerCharacter))
						{
							if (PlayerCharacter->GetController()->IsPlayerController())
							{
								OwnerComp.GetBlackboardComponent()->SetValueAsObject(EnemyAIController->TargetActorKey, PlayerCharacter);

								//Debug용 코드
								// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Detected!")));
								// DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Red, false, 0.5f);
								// DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Red, false, 0.5f);
								// DrawDebugLine(World, ActionAICharacter->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Red, false, 0.5f, 0u, 3.f);

								return;
							}
						}

						OwnerComp.GetBlackboardComponent()->SetValueAsObject(EnemyAIController->TargetActorKey, nullptr);
						
						//DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Cyan, false, 0.5f);
					}
				}
				else
				{
					// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Not Detected!")));
					// OwnerComp.GetBlackboardComponent()->SetValueAsObject(EnemyAIController->TargetActorKey, nullptr);
					// DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Cyan, false, 0.5f);
				}
			}
		}
	}
}

