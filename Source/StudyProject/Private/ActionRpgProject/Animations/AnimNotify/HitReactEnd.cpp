// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/HitReactEnd.h"

#include "ActionRpgProject/Characters/BaseCharacter.h"

void UHitReactEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp->GetOwner()))
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(IsValid(BaseCharacter))
		{
			BaseCharacter->SetActionState(EActionState::EAS_None);
		}
	}
}
