// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/UpdateTranslationTarget.h"

#include "ActionRpgProject/Characters/BaseCharacter.h"


void UUpdateTranslationTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp->GetOwner()))
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(IsValid(BaseCharacter))
		{
			BaseCharacter->WarpToTarget();
		}
	}
}
