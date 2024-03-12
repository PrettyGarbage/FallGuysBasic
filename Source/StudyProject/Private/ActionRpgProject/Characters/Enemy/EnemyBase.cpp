﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Characters/Enemy/EnemyBase.h"

#include "AIController.h"
#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/Define/DefineVariables.h"
#include "ActionRpgProject/HUD/HealthBarComponent.h"
#include "ActionRpgProject/Items/SwordWeapon.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensingComponent->SightRadius = 4000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetHealthPercentage(AttributeComponent->GetHealthPercent());
		HealthBarWidget->SetVisibility(false);
	}

	AIController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);

	if(IsValid(PawnSensingComponent))
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyBase::PawnSeen);
	}

	EnemyState = EEnemyState::EES_Patrolling;

	UWorld* World = GetWorld();
	if(IsValid(World) && IsValid(WeaponClass))
	{
		ASwordWeapon* DefaultWeapon = World->SpawnActor<ASwordWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), GRightHandSocket, this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemyBase::Die()
{
	Super::Die();
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(IsValid(AnimInstance) && IsValid(DeathMontage))
	{
		AnimInstance->Montage_Play(DeathMontage);
		const int32 Selection = FMath::RandRange(1, 4);
		const FName SectionName = FName(*FString::Printf(TEXT("Death%d"), Selection));
		DeathPose = GetDeathPoseEnumValue(SectionName);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	if(IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetVisibility(false);
	}

	EnemyState = EEnemyState::EES_Dead; 
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
}

double AEnemyBase::GetTargetDistance() const
{
	if(!IsValid(CombatTarget)) return DBL_MAX;
	
	return FVector::Dist(GetActorLocation(), CombatTarget->GetActorLocation()); 
}

bool AEnemyBase::IsTargetInRange(AActor* InTarget, double Radius)
{
	if(!IsValid(InTarget)) return false;
	const double Distance = FVector::Distance(InTarget->GetActorLocation(), GetActorLocation());
	DrawDebugSphere(GetWorld(), InTarget->GetActorLocation(), Radius, 12, FColor::Red, false, 0.1f);
	return Distance <= Radius;
}

void AEnemyBase::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}


void AEnemyBase::CheckCombatTarget()
{
	if(!IsTargetInRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		if(IsValid(HealthBarWidget))
		{
			HealthBarWidget->SetVisibility(false);
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
	}
	else if (!IsTargetInRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
	}
	else if (IsTargetInRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Attacking;
		AIAttack();
	}
}

void AEnemyBase::CheckPatrolTarget()
{
	if(IsTargetInRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::PatrolTimerFinished, WaitTime);
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(EnemyState == EEnemyState::EES_Attacking || EnemyState  == EEnemyState::EES_Chasing)
	{
		CheckCombatTarget();
	}
	else if (EnemyState == EEnemyState::EES_Patrolling)
	{
		CheckPatrolTarget();
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AEnemyBase::GetHit_Implementation(const FVector& ImpactPoint)
{
	//첫 타격시에 UI가 노출되도록
	if(IsValid(HealthBarWidget) && !HealthBarWidget->IsVisible())
	{
		HealthBarWidget->SetVisibility(true);
	}
	
	if(IsValid(AttributeComponent) && AttributeComponent->IsAlive())
	{
		DirectionHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
	
	if(IsValid(HitSound))
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	//const라서 IsValid가 안됨.
	if(HitParticle && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			ImpactPoint
		);
	}
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(IsValid(AttributeComponent) && IsValid(HealthBarWidget))
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		HealthBarWidget->SetHealthPercentage(AttributeComponent->GetHealthPercent());
	}

	if(IsValid(EventInstigator))
	{
		CombatTarget = EventInstigator->GetPawn();
	}

	EnemyState = EEnemyState::EES_Chasing;
	MoveToTarget(CombatTarget);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	return DamageAmount;
}

void AEnemyBase::Destroyed()
{
	Super::Destroyed();

	if(IsValid(EquippedWeapon))
	{
		EquippedWeapon->Destroy();
	}
}

EDeathPose AEnemyBase::GetDeathPoseEnumValue(FName InName)
{
	if(!DeathPoseMap.Contains(InName))
	{
		UE_LOG(LogTemp, Error, TEXT("DeathPoseMap does not contain %s"), *InName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DeathPoseMap contains %s"), *InName.ToString());
	}
	
	return DeathPoseMap.Contains(InName) ? DeathPoseMap[InName] : EDeathPose::EDP_Death1;
}

TObjectPtr<AActor> AEnemyBase::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for(auto Target : PatrolTargets)
	{
		if(Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
			
	const int32 NumPatrolTargets = ValidTargets.Num();
	if(NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemyBase::AIAttack()
{
	PlayAttackMontage();
}


void AEnemyBase::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(IsValid(AnimInstance) && IsValid(AttackMontage))
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName(GAttackString + FString::FromInt(Selection));

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEnemyBase::PawnSeen(APawn* SeenPawn)
{
	if(EnemyState == EEnemyState::EES_Chasing) return;
	
	if(IsValid(SeenPawn) && SeenPawn->ActorHasTag(GTag_Player))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;

		if(EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}
	}
}

void AEnemyBase::MoveToTarget(AActor* InTarget)
{
	if(!IsValid(AIController) || !IsValid(InTarget)) return;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(InTarget);
	MoveRequest.SetAcceptanceRadius(50.f);
	AIController->MoveTo(MoveRequest);
}



