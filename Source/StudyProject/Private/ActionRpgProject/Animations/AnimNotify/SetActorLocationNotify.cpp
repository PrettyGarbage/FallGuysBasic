// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Animations/AnimNotify/SetActorLocationNotify.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void USetActorLocationNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(IsValid(MeshComp))
	{
		AActionCharacter* BindCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
		if(IsValid(BindCharacter))
		{
			//BindCharacter의 위치에서 캐릭터가 바라보는 방향으로 MeshComp의 RootBoneTranslation만큼 떨어진 위치를 구해서 변수에 할당한다.
			FVector RootBoneLocation = BindCharacter->GetActorLocation() + BindCharacter->GetActorForwardVector() * MeshComp->RootBoneTranslation.Y;
			//루트본 오프셋을 초기화
			MeshComp->RootBoneTranslation = FVector::ZeroVector;
			BindCharacter->SetActorLocation(RootBoneLocation);
		}
	}
}
