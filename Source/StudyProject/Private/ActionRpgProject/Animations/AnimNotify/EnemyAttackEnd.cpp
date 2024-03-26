// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/EnemyAttackEnd.h"

#include "ActionRpgProject/Characters/Enemy/ActionAICharacter.h"
#include "ActionRpgProject/Characters/Enemy/EnemyBase.h"

void UEnemyAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp))
	{
		AEnemyBase* BindCharacter = Cast<AEnemyBase>(MeshComp->GetOwner());
		if(IsValid(BindCharacter))
		{
			BindCharacter->SetEnemyState(EEnemyState::EES_Chasing);
		}

		AActionAICharacter* AICharacter = Cast<AActionAICharacter>(MeshComp->GetOwner());
		if(IsValid(AICharacter))
		{
			AICharacter->SetHitReactEndState();
		}
	}
}
