// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/AttackEndNotify.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"


void UAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp))
	{
		AActionCharacter* AttackCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
		if(IsValid(AttackCharacter))
		{
			AttackCharacter->SetCharacterActionState(EActionState::EAS_None);
		}
	}
}
