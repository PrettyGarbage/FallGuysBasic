// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/AttackEndNotify.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"


void UAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp))
	{
		ABaseCharacter* BindCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(IsValid(BindCharacter))
		{
			BindCharacter->SetActionState(EActionState::EAS_None);
		}
	}
}
