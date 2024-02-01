// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/AN_PlayMuzzleEffect.h"

void UAN_PlayMuzzleEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	
}
