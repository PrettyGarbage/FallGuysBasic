// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "AN_PlayMuzzleEffect.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UAN_PlayMuzzleEffect : public UAnimNotify_PlayParticleEffect
{
	GENERATED_BODY()

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
