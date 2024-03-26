// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/HitReactEndNotify.h"

#include "ActionRpgProject/Characters/Enemy/ActionAICharacter.h"

void UHitReactEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp))
	{
		AActionAICharacter* BindCharacter = Cast<AActionAICharacter>(MeshComp->GetOwner());
		if(IsValid(BindCharacter))
		{
			BindCharacter->SetHitReactEndState();
		}
	}
}
