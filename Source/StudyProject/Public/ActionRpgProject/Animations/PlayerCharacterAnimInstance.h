// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, Category = CharacterAnimInstance, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AActionCharacter> PlayerCharacter;

	UPROPERTY(VisibleAnywhere, Category = CharacterAnimInstance, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterAnimInstance, meta = (AllowPrivateAccess = true))
	float GroundSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category=CharacterAnimInstance, meta=(AllowPrivateAccess = true))
	uint8 IsFalling : 1;
};
