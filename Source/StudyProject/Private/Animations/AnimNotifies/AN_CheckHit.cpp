// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/AN_CheckHit.h"
#include "Characters/SRPGCharacter.h"


void UAN_CheckHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp))
	{
		ASRPGCharacter* AttackCharacter = Cast<ASRPGCharacter>(MeshComp->GetOwner());
		if(IsValid(AttackCharacter))
		{
			AttackCharacter->CheckHit();
		}
	}
}
