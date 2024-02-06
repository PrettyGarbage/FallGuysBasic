// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/Animations/STPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPSStudyProject/Characters/STPSCharacter.h"

USTPSAnimInstance::USTPSAnimInstance() : bIsDead(false)
{
	
}

void USTPSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ASTPSCharacter>(GetOwningActor());
	if(IsValid(OwnerCharacter))
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void USTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(IsValid(MovementComponent) && IsValid(OwnerCharacter))
	{
		GroundSpeed = MovementComponent->GetMaxSpeed();

		float ForwardInputValue = fabs(MovementComponent->Velocity.X) * OwnerCharacter->GetForwardInputValue();
		float RightInputValue = fabs(MovementComponent->Velocity.Y) * OwnerCharacter->GetRightInputValue();
		float UpInputValue = MovementComponent->Velocity.Z;
		MoveInputWithMaxSpeed = FVector{ForwardInputValue, RightInputValue, UpInputValue};

		float X = fabs(MoveInputWithMaxSpeed.X) < KINDA_SMALL_NUMBER ? 0.f : MoveInputWithMaxSpeed.X / fabs(MoveInputWithMaxSpeed.X);
		float Y = fabs(MoveInputWithMaxSpeed.Y) < KINDA_SMALL_NUMBER ? 0.f : MoveInputWithMaxSpeed.Y / fabs(MoveInputWithMaxSpeed.Y);
		float Z = fabs(MoveInputWithMaxSpeed.Z) < KINDA_SMALL_NUMBER ? 0.f : MoveInputWithMaxSpeed.Z / fabs(MoveInputWithMaxSpeed.Z);
		MoveInput = FVector{X, Y, Z};

		bIsFalling = MovementComponent->IsFalling();

		ControlRotation.Pitch = OwnerCharacter->GetCurrentAimPitch();
		ControlRotation.Yaw = OwnerCharacter->GetCurrentAimYaw();
	}
}
