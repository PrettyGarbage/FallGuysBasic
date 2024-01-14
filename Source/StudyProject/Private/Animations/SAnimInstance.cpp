// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/SAnimInstance.h"
#include "Characters/SRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

USAnimInstance::USAnimInstance()
{
}

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CurrentSpeed = 0.f;

	Velocity = FVector::ZeroVector;

	bIsFalling = false;

	bIsCrouching = false;
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ASCharacter* OwnerCharacter = Cast<ASCharacter>(TryGetPawnOwner());
	if (true == IsValid(OwnerCharacter))
	{
		UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
		if (true == IsValid(CharacterMovementComponent))
		{
			Velocity = CharacterMovementComponent->GetLastUpdateVelocity();
			CurrentSpeed = Velocity.Size();
			bIsFalling = CharacterMovementComponent->IsFalling();
			bIsCrouching = CharacterMovementComponent->IsCrouching();
		}
	}
}

void USAnimInstance::PlayAttackMontage()
{
	if(true == IsValid(AttackAnimMontage))
	{
		if(false == Montage_IsPlaying(AttackAnimMontage))
		{
			Montage_Play(AttackAnimMontage);
		}
	}
}

void USAnimInstance::AnimNotify_CheckHit() const
{
	if(true == OnCheckHitDelegate.IsBound())
	{
		OnCheckHitDelegate.Broadcast();
	}
}

void USAnimInstance::AnimNotify_CheckCanNextCombo() const
{
	if(true == OnCheckCanNextComboDelegate.IsBound())
	{
		OnCheckCanNextComboDelegate.Broadcast();
	}
}
