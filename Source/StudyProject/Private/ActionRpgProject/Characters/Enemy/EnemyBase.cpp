// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Characters/Enemy/EnemyBase.h"

#include "AIController.h"
#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/Define/DefineVariables.h"
#include "ActionRpgProject/HUD/HealthBarComponent.h"
#include "ActionRpgProject/Items/SwordWeapon.h"
#include "ActionRpgProject/Items/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead()) return;
	
	if(EnemyState == EEnemyState::EES_Attacking || EnemyState  == EEnemyState::EES_Chasing)
	{
		CheckCombatTarget();
	}
	else if (EnemyState == EEnemyState::EES_Patrolling)
	{
		CheckPatrolTarget();
	}
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	if(IsAlive())
	{
		ShowHealthBarToggle(true);
	}
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	
	StopAttackMontage();
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);

	if(IsValid(EventInstigator))
	{
		CombatTarget = EventInstigator->GetPawn();
	}

	if(IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		ChaseTarget();
	}

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

void AEnemyBase::SetEnemyState(EEnemyState InState)
{
	EnemyState = InState;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsValid(PawnSensingComponent))
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyBase::PawnSeen);
	}

	InitializeEnemy();
	Tags.Add(GTag_Enemy);
}

void AEnemyBase::Die()
{
	Super::Die();
	
	EnemyState = EEnemyState::EES_Dead;
	ShowHealthBarToggle(false);
	PlayDeathMontage();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;

	SpawnTreasure();
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
	//DrawDebugSphere(GetWorld(), InTarget->GetActorLocation(), Radius, 12, FColor::Red, false, 0.1f);
	return Distance <= Radius;
}

void AEnemyBase::MoveToTarget(AActor* InTarget)
{
	if(!IsValid(AIController) || !IsValid(InTarget)) return;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(InTarget);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	AIController->MoveTo(MoveRequest);
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

void AEnemyBase::Attack()
{
	Super::Attack();

	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
	UKismetSystemLibrary::PrintString(GetWorld(), "Attack", true, true, FLinearColor::Red, 5.f);
}

bool AEnemyBase::CanAttack()
{
	const bool bCanAttack =
		IsInsideAttackRadius() &&
			!IsAttacking() &&
			!IsEngaged() &&
			!IsDead();
	return bCanAttack;
}

void AEnemyBase::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if(IsValid(AttributeComponent) && IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetHealthPercentage(AttributeComponent->GetHealthPercent());
	}
}

void AEnemyBase::PawnSeen(APawn* SeenPawn)
{
	if(EnemyState == EEnemyState::EES_Chasing) return;

	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
			EnemyState < EEnemyState::EES_Attacking &&
				IsValid(SeenPawn) &&
					SeenPawn->ActorHasTag(GTag_Player);
				
	if(bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}

void AEnemyBase::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemyBase::CheckCombatTarget()
{
	if(IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		SetEnemyInterest(nullptr);
		if(!IsEngaged())
		{
			StartPatrolling();
		}
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if(!IsEngaged())
		{
			ChaseTarget();
		}
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemyBase::CheckPatrolTarget()
{
	if(IsTargetInRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::PatrolTimerFinished, WaitTime);
	}
}

void AEnemyBase::InitializeEnemy()
{
	AIController = Cast<AAIController>(GetController());
	EnemyState = EEnemyState::EES_Patrolling;
	ShowHealthBarToggle(false);
	MoveToTarget(PatrolTarget);
	SpawnDefaultWeapon();
}

void AEnemyBase::ShowHealthBarToggle(bool bShow)
{
	if(IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetVisibility(bShow);
	}
}

void AEnemyBase::SetEnemyInterest(AActor* InActor)
{
	if(!IsValid(InActor))
	{
		CombatTarget = nullptr;
		ShowHealthBarToggle(false);
	}
}

void AEnemyBase::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemyBase::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemyBase::IsOutsideCombatRadius()
{
	return !IsTargetInRange(CombatTarget, CombatRadius);
}

bool AEnemyBase::IsOutsideAttackRadius()
{
	return !IsTargetInRange(CombatTarget, AttackRadius);
}

bool AEnemyBase::IsInsideAttackRadius()
{
	return IsTargetInRange(CombatTarget, AttackRadius);
}

bool AEnemyBase::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemyBase::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemyBase::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemyBase::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemyBase::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemyBase::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyBase::Attack, AttackTime);
}

void AEnemyBase::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemyBase::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if(IsValid(World) && IsValid(WeaponClass))
	{
		ASwordWeapon* DefaultWeapon = World->SpawnActor<ASwordWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), GWeaponSocket, this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemyBase::SpawnTreasure()
{
	UWorld* World = GetWorld();
	if(IsValid(World)&& IsValid(TreasureClass))
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 25.f);
		World->SpawnActor<ATreasure>(TreasureClass, SpawnLocation, GetActorRotation());
	}
}






