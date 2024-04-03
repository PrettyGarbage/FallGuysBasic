// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/BossAttackHitCheck.h"

#include "ActionRpgProject/Characters/Enemy/ActionBossAICharacter.h"

void UBossAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp))
	{
		AActionBossAICharacter* AttackCharacter = Cast<AActionBossAICharacter>(MeshComp->GetOwner());
		if(IsValid(AttackCharacter))
		{
			AttackCharacter->CheckHit();
		}
	}
}
