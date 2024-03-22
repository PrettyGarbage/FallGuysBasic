// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Characters/BaseCharacter.h"

#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/Define/DefineVariables.h"
#include "ActionRpgProject/Items/SwordWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "ActionRpgProject/Characters/Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpComponent"));
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type InType)
{
	if(IsValid(EquippedWeapon) && IsValid(EquippedWeapon->GetWeaponBox()))
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(InType);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::SetActionState(EActionState InState)
{
	CurrentActionState = InState;
}

void ABaseCharacter::WarpToTarget()
{
	if(!IsValid(MotionWarpComponent)) return;

	ABaseCharacter* TargetActor = Cast<ABaseCharacter>(CombatTarget);
	if(IsValid(CombatTarget) && IsValid(TargetActor))
	{
		MotionWarpComponent->AddOrUpdateWarpTargetFromLocation(
			GTranslationTarget,
			TargetActor->GetTranslationWarpTarget(TargetActor));
	}
}



void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	IHitInterface::GetHit_Implementation(ImpactPoint, Hitter);

	if(IsAlive() && Hitter)
	{
		DirectionHitReact(Hitter->GetActorLocation());
	}
	else
	{
		Die();
	}

	PlayHitSound(ImpactPoint);
	SpawnHitParticle(ImpactPoint);
}

void ABaseCharacter::Attack(const FInputActionValue& InValue)
{
	if(IsValid(CombatTarget))
	{
		ABaseCharacter* TargetActor = Cast<ABaseCharacter>(CombatTarget);
		if(IsValid(TargetActor))
		{
			if(TargetActor->IsAlive())
			{
				CombatTarget = nullptr;
			}
		}
	}
}

void ABaseCharacter::Attack()
{
	if(IsValid(CombatTarget))
	{
		ABaseCharacter* TargetActor = Cast<ABaseCharacter>(CombatTarget);
		if(IsValid(TargetActor))
		{
			if(TargetActor->IsAlive())
			{
				CombatTarget = nullptr;
			}
		}
	}
}

void ABaseCharacter::Die()
{
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	DisableCapsule();
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
		SectionName = GFromFront;
	}
	else if(Theta >= -135.f && Theta < -45.f)
	{
		SectionName = GFromLeft;
	}
	else if(Theta >= 45.f && Theta < 135.f)
	{
		SectionName = GFromRight;
	}
	else
	{
		SectionName = GFromBack;
	}
	
	PlayHitReactMontage(SectionName);

	// 디버그용
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation()+ Cross * 100.f,5.f, FColor::Red, 5.f, 1.f);
	// if(IsValid(GEngine))
	// {
	// 	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta : %f"), Theta));
	// }
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation()+ Forward * 60.f,5.f, FColor::Red, 5.f, 1.f);
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation()+ ToHit * 60.f,5.f, FColor::Red, 5.f, 1.f);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if(IsValid(HitSound))
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticle(const FVector& ImpactPoint)
{
	if(HitParticle && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			ImpactPoint
		);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if(IsValid(AttributeComponent))
	{
		AttributeComponent->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if(IsValid(AnimInstance) && IsValid(Montage))
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if(SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 SectionIndex = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[SectionIndex]);
	return SectionIndex;
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	EDeathPose Pose = static_cast<EDeathPose>(Selection);
	if(Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	
	return Selection;
}

void ABaseCharacter::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, GDodge);
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


bool ABaseCharacter::IsAlive()
{
	return IsValid(AttributeComponent) && AttributeComponent->IsAlive();
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

FVector ABaseCharacter::GetTranslationWarpTarget(ABaseCharacter* InTarget)
{
	if(!IsValid(InTarget)) return FVector();

	const FVector CombatTargetLocation = InTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();

	TargetToMe *= WarpTargetDistance;

	//Debug
	DrawDebugSphere(GetWorld(), CombatTargetLocation + TargetToMe, 10.f, 12, FColor::Red, false, 0.1f);

	return CombatTargetLocation + TargetToMe;
}




