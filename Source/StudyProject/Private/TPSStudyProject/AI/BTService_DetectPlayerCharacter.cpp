// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/AI/BTService_DetectPlayerCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "TPSStudyProject/Characters/SNonPlayerCharacter.h"
#include "TPSStudyProject/Controllers/SAIController.h"
#include "Kismet/KismetSystemLibrary.h"

UBTService_DetectPlayerCharacter::UBTService_DetectPlayerCharacter()
{
	NodeName = TEXT("DetectPlayerCharacter");
	Interval = 1.f;
}

void UBTService_DetectPlayerCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	if(IsValid(AIController))
	{
		ASNonPlayerCharacter* NonPlayerCharacter = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
		if(IsValid(NonPlayerCharacter))
		{
			UWorld* World = NonPlayerCharacter->GetWorld();
			if(IsValid(World))
			{
				FVector CenterPosition = NonPlayerCharacter->GetActorLocation();
				float DetectRadius = 300.f;
				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams  CollisionQueryParams(NAME_None, false, NonPlayerCharacter);
				bool bResult = World->OverlapMultiByChannel(OverlapResults, CenterPosition, FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel12, FCollisionShape::MakeSphere(DetectRadius),
					CollisionQueryParams);

				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Value Test! : %d"),bResult));
				if(bResult)
				{
					for(auto const& OverlapResult : OverlapResults)
					{
						ASCharacter* PlayerCharacter = Cast<ASCharacter>(OverlapResult.GetActor());
						if(IsValid(PlayerCharacter))
						{
							if (PlayerCharacter->GetController()->IsPlayerController())
							{
								OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, PlayerCharacter);

								UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Detected!")));
								DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Red, false, 0.5f);
								DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Red, false, 0.5f);
								DrawDebugLine(World, NonPlayerCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Red, false, 0.5f, 0u, 3.f);

								return;
							}
						}

						OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, nullptr);

						DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Cyan, false, 0.5f);
					}
				}
				else
				{
					UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Not Detected!")));
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASAIController::TargetActorKey, nullptr);
					DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Cyan, false, 0.5f);
				}
			}
		}
	}

	
}
