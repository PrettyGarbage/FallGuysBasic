// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Characters/BaseCharacter.h"

#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/Items/SwordWeapon.h"
#include "Components/BoxComponent.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack(const FInputActionValue& InValue)
{
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(IsValid(AnimInstance) && IsValid(HitReactMontage))
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionHitReact(const FVector& ImpactPoint)
{
	//내적을 이용하여 피격방향을 구한다.
	const FVector Forward = GetActorForwardVector();
	//높이에 대한 부분은 무시한다.
	const FVector ImpactHeight = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	//normalize를 통해 각도를 구할 수 맞은 방향의 방향 벡터를 구한다.
	const FVector ToHit = (ImpactHeight - GetActorLocation()).GetSafeNormal();
	//Forward * ToHit = |Forward| * |ToHit| * cos(각도)
	//|Forward| = 1, |ToHit| = 1이므로(두 단위벡터가 평행하면 절대값은 1이다) cos(각도) = Forward * ToHit
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//acos를 통해 각도만 남기도록 한다.
	double Theta = FMath::Acos(CosTheta);
	//라디안을 각도로 변환한다.
	Theta = FMath::RadiansToDegrees(Theta);

	//오른손 법칙을 이용한 보정 코사인 함수가 주기함수라 방향이 잘못나오기도 한다고 함...
	const FVector Cross = FVector::CrossProduct(Forward, ToHit);

	if(Cross.Z < 0)
	{
		Theta *= -1.f;
	}

	FName SectionName;

	if(Theta >= -45.f && Theta < 45.f)
	{
		SectionName = FName("FromFront");
	}
	else if(Theta >= -135.f && Theta < -45.f)
	{
		SectionName = FName("FromLeft");
	}
	else if(Theta >= 45.f && Theta < 135.f)
	{
		SectionName = FName("FromRight");
	}
	else
	{
		SectionName = FName("FromBack");
	}
	
	PlayHitReactMontage(SectionName);

	// 디버그용
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation()+ Cross * 100.f,5.f, FColor::Red, 5.f, 1.f);
	//
	// if(IsValid(GEngine))
	// {
	// 	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta : %f"), Theta));
	// }
	//
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation()+ Forward * 60.f,5.f, FColor::Red, 5.f, 1.f);
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation()+ ToHit * 60.f,5.f, FColor::Red, 5.f, 1.f);
}

void ABaseCharacter::PlayAttackMontage()
{
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type InType)
{
	if(IsValid(EquippedWeapon) && IsValid(EquippedWeapon->GetWeaponBox()))
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(InType);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

