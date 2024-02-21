// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/PlayerCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<AActionCharacter>(TryGetPawnOwner());
	if(IsValid(PlayerCharacter))
	{
		CharacterMovementComponent = PlayerCharacter->GetCharacterMovement();
	}
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(IsValid(PlayerCharacter) && IsValid(CharacterMovementComponent))
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);
		IsFalling = CharacterMovementComponent->IsFalling();
		CharacterState = PlayerCharacter->GetCharacterState();
	}
}
