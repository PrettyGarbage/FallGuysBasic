// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/DisarmNotify.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"

void UDisarmNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp))
	{
		AActionCharacter* BindCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
		if(IsValid(BindCharacter))
		{
			BindCharacter->Disarm();
		}
	}
}
