// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/CheckAttackPressedKey.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"

void UCheckAttackPressedKey::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp))
	{
		AActionCharacter* ActionCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
		if(IsValid(ActionCharacter))
		{
			ActionCharacter->CheckCanNextCombo();
		}
	}
}
