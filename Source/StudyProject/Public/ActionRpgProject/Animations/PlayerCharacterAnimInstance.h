// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRpgProject/Characters/ActionCharacter.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	float GroundSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta=(AllowPrivateAccess = true))
	uint8 IsFalling : 1;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta=(AllowPrivateAccess = true))
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category= Movement, meta=(AllowPrivateAccess = true))
	EDeathPose DeathPose;
};
