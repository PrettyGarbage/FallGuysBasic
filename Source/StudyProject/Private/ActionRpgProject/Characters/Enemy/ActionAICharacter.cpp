// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Characters/Enemy/ActionAICharacter.h"

#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/Controller/EnemyAIController.h"
#include "ActionRpgProject/HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AActionAICharacter::AActionAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AActionAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if(IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

	InitializeAnimMontageEvent();
	ShowHPbar(false);
}

void AActionAICharacter::SetHitReactEndState()
{
	bHitReact = false;
	bIsAttacking = false;
}

void AActionAICharacter::CallAttackLogic()
{
	Attack();
}

float AActionAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float FinalDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	bHitReact = true;
	bIsAttacking = false;
	
	ShowHPbar(true);
	HandleDamage(DamageAmount);

	if(IsValid(EventInstigator))
	{
		CombatTarget = EventInstigator->GetPawn();
	}
	
	return DamageAmount;
}

void AActionAICharacter::InitializeAnimMontageEvent()
{
	// UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//  if(IsValid(AnimInstance))
	//  {
	//  	if(!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &AActionAICharacter::OnAttackMontageEnded))
	//  	{
	//  		AnimInstance->OnMontageEnded.AddDynamic(this, &AActionAICharacter::OnAttackMontageEnded);
	//  	}
	//  }
}

void AActionAICharacter::Attack()
{
	if(bIsAttacking) return;
	
	Super::Attack();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + AttackRange * GetActorForwardVector(),
		FQuat::Identity,
		ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if(IsValid(AttackMontage) && AttackMontageSections.Num() > 0)
	{
		const int32 SectionIndex = FMath::RandRange(0, AttackMontageSections.Num() - 1);
		bIsAttacking = true;
		PlayMontageSection(AttackMontage, AttackMontageSections[SectionIndex]);
	}
}

void AActionAICharacter::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	
	if(IsValid(AttributeComponent) && IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetHealthPercentage(AttributeComponent->GetHealthPercent());
		if(AttributeComponent->GetHealthValue() < KINDA_SMALL_NUMBER)
		{
			ShowHPbar(false);

			AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());
			if(IsValid(EnemyAIController))
			{
				EnemyAIController->EndAI();
				EnemyState = EEnemyState::EES_Dead;
				PlayDeathMontage();
				SetLifeSpan(2.f);
			}
		}
	}
}


void AActionAICharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
}

void AActionAICharacter::ShowHPbar(bool bShow)
{
	if(IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetVisibility(bShow);
	}
}


